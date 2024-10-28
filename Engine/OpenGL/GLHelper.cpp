#include "GLHelper.h"

#include <iostream>

#include "../Editor/Editor.h"
#include "../Camera/Camera.h"

GLint GLHelper::width;
GLint GLHelper::height;
GLdouble GLHelper::fps;
GLdouble GLHelper::delta_time;
std::string GLHelper::title;
GLFWwindow* GLHelper::ptr_window;

std::map<int, GLboolean> GLHelper::keyState;
std::stack<int> GLHelper::keyStateStack;
std::map<int, GLboolean> GLHelper::ctrlKeyState;

glm::vec2 GLHelper::mousePos;
GLboolean GLHelper::mousestateLeft;

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------

bool GLHelper::Init(GLint _width, GLint _height, std::string _title) 
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
        return false;
    }
    glfwMakeContextCurrent(ptr_window);

    setup_event_callbacks();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthRange(0, 1);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0);

    glLineWidth(5);

    return true;
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
    if (_action == GLFW_PRESS) 
    {
        if (GLFW_KEY_ESCAPE == _key && Editor::GetInstance().GetMode() == Editor::PLAY) 
        {
            glfwSetWindowShouldClose(_pwin, GLFW_TRUE);
        }

        keyState[_key] = GL_TRUE;

        if (keyStateStack.size() == 1 && keyStateStack.top() == GLFW_KEY_LEFT_CONTROL)
        {
            ctrlKeyState[_key] = GL_TRUE;
        }

        keyStateStack.push(_key);
    }
    else if (_action == GLFW_RELEASE) 
    {
        keyState[_key] = GL_FALSE;
        
        while (!keyStateStack.empty() && !keyState[keyStateStack.top()])
        {
            if (ctrlKeyState[keyStateStack.top()])
            {
                ctrlKeyState[keyStateStack.top()] = GL_FALSE;
            }

            keyStateStack.pop();
        }
    }
}

void GLHelper::mousebutton_cb(GLFWwindow* _pwin, int _button, int _action, int _mod) 
{
    if (_action == GLFW_PRESS) 
    {
        mousestateLeft = (_button == GLFW_MOUSE_BUTTON_LEFT) ? GL_TRUE : mousestateLeft;
    }
    else if (_action == GLFW_RELEASE) 
    {
        mousestateLeft = (_button == GLFW_MOUSE_BUTTON_LEFT) ? GL_FALSE : mousestateLeft;
    }
}

void GLHelper::mousepos_cb(GLFWwindow* _pwin, double _xpos, double _ypos) 
{
    glm::vec2 cameraPos = Camera::GetInstance().GetPos();
    float cameraHeight = Camera::GetInstance().GetHeight();

    mousePos.x = (_xpos - (width / 2.)) * cameraHeight + cameraPos.x;
    mousePos.y = -(_ypos - (height / 2.)) * cameraHeight + cameraPos.y;
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