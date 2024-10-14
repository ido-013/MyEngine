#include "Editor.h"

#include "../OpenGL/GLHelper.h"

#include "../GameObjectManager/GameObjectManager.h"
#include "../Serializer/Serializer.h"
#include "../RTTI/Registry.h"
#include "../Components.h"

Editor::Editor() : buffer(), selected(nullptr)
{
    comps =
    {
        TransformComp::TypeName,
        SpriteComp::TypeName
    };
}

Editor::~Editor()
{

}

void Editor::ClearBuffer()
{
    for (char& c : buffer)
    {
        c = '\0';
    }
}

void Editor::TopBar()
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("New GameObject"))
    {
        ImGui::InputText("Name", buffer, 100);

        if (ImGui::Button("Create"))
        {
            GameObjectManager::GetInstance().CreateObject(buffer);
            ClearBuffer();
        }

        ImGui::EndMenu();
    }

    if (ImGui::Button("New Level"))
    {
        selected = nullptr;

        GameObjectManager::GetInstance().RemoveAllObject();
    }

    if (ImGui::BeginMenu("Load Level"))
    {
        ImGui::InputText("Name", buffer, 100);

        if (ImGui::Button("Load"))
        {
            selected = nullptr;

            Serializer::GetInstance().LoadLevel(buffer);
            ClearBuffer();
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Save Level"))
    { 
        ImGui::InputText("Name", buffer, 100);

        if (ImGui::Button("Save"))
        {
            Serializer::GetInstance().SaveLevel(buffer);
            ClearBuffer();
        }

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void Editor::GameObjectList()
{
    ImGui::Begin("Game Object List");

    for (auto& it : GameObjectManager::GetInstance().GetAllObject())
    {
        if (ImGui::Button(it.first.c_str()))
        {
            selected = it.second;
        }
    }

    ImGui::End();
}

void Editor::SelectedGameObjectInfo()
{
    if (selected == nullptr)
        return;

    ImGui::Begin("Game Object");

    if (ImGui::TreeNode("Add Component"))
    {
        for (auto& compType : comps) 
        {
            if (ImGui::Button(compType.c_str()))
            {
                Registry::GetInstance().FindAndCreate(compType, selected);
            }
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Component"))
    {
        for (auto& it : selected->GetAllComponent())
        {
            if (ImGui::Button(it.first.c_str()))
            {
                
            }
        }

        ImGui::TreePop();
    }

    if (ImGui::Button("Delete"))
    {
        GameObjectManager::GetInstance().RemoveObject(selected->GetName());
        selected = nullptr;
    }

    ImGui::End();
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
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::ShowDemoWindow(); // Show demo window! :)

    TopBar();
    GameObjectList();
    SelectedGameObjectInfo();

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
