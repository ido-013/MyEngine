#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "OpenGL/GLHelper.h"
#include "GameStateManager/GameStateManager.h"
#include "Level/TestLevel.h"

#include <iostream>

void TopBar()
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
        
        // private
        char buffer[100] = { "\0" };

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

//ShowAllObjects() - f
    //GameObject* selected (private)

//SelectedObjectWindow() - f
    //AddComponent
    //TODO: Move to factory
    //if(ImGui::TreeNode("Add Component"))
    //std::vector<std::string> comps = {Transform::GetName(), Sprite::GetName()};
    //for (auto& compType : comps) {if (ImGui::Button(compTyp.c_str())) { selected->CreateComponent(compType); }
    //SeleteComponent
        //Call Component Edit function (virtual bool Edit() = 0)
        //Imgui::TreeNode()
        //if (ComponentBase::Edit()) { } -> override
        //Transform->Pos
            //ImGui::InputFloat2("Pos", &pos[0]);
            //
            //float t= glm::degrees(rotation);
            //ImGui::SliderFloat("Rot", &t, -360, 360); 
            //rotation = glm::radians(t);
            //
        //Imgui::TreePop();
    // 
    //DeleteComponent
    //Add delete button to ComponentBase::Edit()
    //find / erase / delete / treePop / return false

//object picking
//different camera
//function key
    //ctrl + '' -> use stack

int main()
{
    GLHelper::Init(1600, 900, "My Game!!!");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(GLHelper::ptr_window, true);      // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    GameStateManager& gsm = GameStateManager::GetInstance();
    gsm.ChangeLevel(new level::TestLevel);

    while (!glfwWindowShouldClose(GLHelper::ptr_window))
    {
        GLHelper::update_time();
        gsm.Update();

        glfwPollEvents();

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

        glfwSwapBuffers(GLHelper::ptr_window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    gsm.Exit();
    GLHelper::Exit();

    return 0;
}