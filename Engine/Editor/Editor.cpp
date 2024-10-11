#include "Editor.h"

#include "../OpenGL/GLHelper.h"

char Editor::buffer[100] = { '\0' };

void Editor::TopBar()
{
    ImGui::BeginMainMenuBar();

    ImGui::Text("Hello World %i", 56);

    if (ImGui::Button("Test Button"))
    {
        //When I press the button, this happens
        std::cout << "Pressed!" << std::endl;
    }

    if (ImGui::BeginMenu("New GameObject"))
    {
        ImGui::Text("New Game Object Menu");

        ImGui::InputText("GO Name", buffer, 100);
        //ImGui::InputText("GO Name", this->buffer, 100);

        if (ImGui::Button("Create"))
        {
            //Create Game Object
            //GameObjectManager::GetInstance().Create(buffer);
        }

        //New Level
            //selected = nullptr
        //Save Level
        //Load Level
            //selected = nullptr

        ImGui::MenuItem("asdf");

        if (ImGui::BeginMenu("GameObject"))
        {
            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    //Enum EditorMode (play, pause, edit)
}

void Editor::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(GLHelper::ptr_window, true);      // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

void Editor::Update()
{
    return;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // Show demo window! :)

    TopBar();

    // Rendering
    // (Your code clears your framebuffer, renders your other stuff etc.)
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Exit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
