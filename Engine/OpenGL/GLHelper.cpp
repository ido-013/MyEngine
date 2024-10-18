#include "GLHelper.h"

#include <iostream>

#include "../Editor/Editor.h"

GLfloat GLHelper::width;
GLfloat GLHelper::height;
GLdouble GLHelper::fps;
GLdouble GLHelper::delta_time;
std::string GLHelper::title;
GLFWwindow* GLHelper::ptr_window;

GLboolean GLHelper::keystateW;
GLboolean GLHelper::keystateA;
GLboolean GLHelper::keystateS;
GLboolean GLHelper::keystateD;

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------

bool GLHelper::Init(GLfloat _width, GLfloat _height, std::string _title) 
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    width = _width;
    height = _height;
    title = _title;

    // glfw window creation
    // --------------------
    ptr_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (ptr_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(ptr_window);

    setup_event_callbacks();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void GLHelper::setup_event_callbacks() 
{
    glfwSetFramebufferSizeCallback(GLHelper::ptr_window, GLHelper::fbsize_cb);
    glfwSetKeyCallback(GLHelper::ptr_window, GLHelper::key_cb);
    glfwSetMouseButtonCallback(GLHelper::ptr_window, GLHelper::mousebutton_cb);
    glfwSetCursorPosCallback(GLHelper::ptr_window, GLHelper::mousepos_cb);
    glfwSetScrollCallback(GLHelper::ptr_window, GLHelper::mousescroll_cb);
}

void GLHelper::Exit() 
{
    glfwTerminate();
}

void GLHelper::key_cb(GLFWwindow* _pwin, int _key, int _scancode, int _action, int _mod) 
{
    if (GLFW_PRESS == _action) {
        if (GLFW_KEY_ESCAPE == _key && Editor::GetInstance().GetMode() == Editor::PLAY) {
            glfwSetWindowShouldClose(_pwin, GLFW_TRUE);
        }

        keystateW = (_key == GLFW_KEY_W) ? GL_TRUE : keystateW;
        keystateA = (_key == GLFW_KEY_A) ? GL_TRUE : keystateA;
        keystateS = (_key == GLFW_KEY_S) ? GL_TRUE : keystateS;
        keystateD = (_key == GLFW_KEY_D) ? GL_TRUE : keystateD;
    }
    else if (GLFW_RELEASE == _action) {
        // key start changes from pressed to released
        keystateW = (_key == GLFW_KEY_W) ? GL_FALSE : keystateW;
        keystateA = (_key == GLFW_KEY_A) ? GL_FALSE : keystateA;
        keystateS = (_key == GLFW_KEY_S) ? GL_FALSE : keystateS;
        keystateD = (_key == GLFW_KEY_D) ? GL_FALSE : keystateD;
    }
}

void GLHelper::mousebutton_cb(GLFWwindow* _pwin, int _button, int _action, int _mod) 
{
   /* switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
#ifdef _DEBUG
        std::cout << "Left mouse button ";
#endif
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
#ifdef _DEBUG
        std::cout << "Right mouse button ";
#endif
        break;
    }
    switch (action) {
    case GLFW_PRESS:
#ifdef _DEBUG
        std::cout << "pressed!!!" << std::endl;
#endif
        break;
    case GLFW_RELEASE:
#ifdef _DEBUG
        std::cout << "released!!!" << std::endl;
#endif
        break;
    }*/
}

void GLHelper::mousepos_cb(GLFWwindow* _pwin, double _xpos, double _ypos) 
{
//#ifdef _DEBUG
//    std::cout << "Mouse cursor position: (" << xpos << ", " << ypos << ")" << std::endl;
//#endif
}

void GLHelper::mousescroll_cb(GLFWwindow* _pwin, double _xoffset, double _yoffset)
{

}

void GLHelper::error_cb(int _error, char const* _description) 
{
#ifdef _DEBUG
    std::cerr << "GLFW error: " << _description << std::endl;
#endif
}

void GLHelper::fbsize_cb(GLFWwindow* _ptr_win, int _width, int _height) 
{
#ifdef _DEBUG
    std::cout << "fbsize_cb getting called!!!" << std::endl;
#endif
    GLHelper::width = _width;
    GLHelper::height = _height;
}

void GLHelper::update_time(double _fps_calc_interval) 
{
    // get elapsed time (in seconds) between previous and current frames
    static double prev_time = glfwGetTime();
    double curr_time = glfwGetTime();
    delta_time = curr_time - prev_time;
    prev_time = curr_time;

    // fps calculations
    static double count = 0.0; // number of game loop iterations
    static double start_time = glfwGetTime();
    // get elapsed time since very beginning (in seconds) ...
    double elapsed_time = curr_time - start_time;

    ++count;

    // update fps at least every 10 seconds ...
    _fps_calc_interval = (_fps_calc_interval < 0.0) ? 0.0 : _fps_calc_interval;
    _fps_calc_interval = (_fps_calc_interval > 10.0) ? 10.0 : _fps_calc_interval;
    if (elapsed_time > _fps_calc_interval) {
        GLHelper::fps = count / elapsed_time;
        start_time = curr_time;
        count = 0.0;
    }
}