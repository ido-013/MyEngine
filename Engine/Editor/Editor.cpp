#include "Editor.h"

#include "../OpenGL/GLHelper.h"

#include "../Imgui/imgui_stdlib.h"

#include "../GameObjectManager/GameObjectManager.h"
#include "../Serializer/Serializer.h"
#include "../RTTI/Registry.h"
#include "../Camera/Camera.h"

#include "Util.h"
#include "../Prefab/Prefab.h"
#include "../Components.h"

Editor::Editor() : buffer(), selected(nullptr), mode()
{
    comps =
    {
        TransformComp::TypeName,
        SpriteComp::TypeName,
        RigidbodyComp::TypeName,
        PlayerComp::TypeName
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

bool Editor::SameLineButton(const char* label)
{
    ImGui::SameLine();
    return ImGui::Button(label);
}

void Editor::ModeChangeWindow()
{
    ImGui::Begin("mode", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

    ImGui::Columns(3, 0, false);

    if (ImGui::Selectable("Play", mode == PLAY)) mode = PLAY;

    ImGui::NextColumn();
    if (ImGui::Selectable("Pause", mode == PAUSE)) mode = PAUSE;

    ImGui::NextColumn();
    if (ImGui::Selectable("Edit", mode == EDIT)) mode = EDIT;

    ImGui::End();
}

void Editor::TopBar()
{
    ImGui::BeginMainMenuBar();

    TopBarLevel();

    ImGui::EndMainMenuBar();
}

void Editor::TopBarLevel()
{
    if (ImGui::BeginMenu("Level"))
    {
        if (ImGui::MenuItem("New Level"))
        {
            selected = nullptr;

            GameObjectManager::GetInstance().RemoveAllObject();
        }

        if (ImGui::BeginMenu("Load Level"))
        {
            if (ImGui::InputText("##", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue) || SameLineButton("Load"))
            {
                selected = nullptr;

                Serializer::GetInstance().LoadLevel(buffer);
                ClearBuffer();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Save Level"))
        {
            if (ImGui::InputText("##", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue) || SameLineButton("Save"))
            {
                Serializer::GetInstance().SaveLevel(buffer);
                ClearBuffer();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

void Editor::GameObjectInfo()
{
    ImGui::Begin("Game Object");

    ObjectList();
    CreateObject();
    
    ImGui::End();
}

void Editor::ObjectList()
{
    if (ImGui::TreeNode("List"))
    {
        for (auto& it : GameObjectManager::GetInstance().GetAllObject())
        {
            if (ImGui::Button(it.first.c_str()))
            {
                selected = it.second;
            }
        }

        ImGui::TreePop();
    }
}

void Editor::CreateObject()
{
    if (ImGui::TreeNode("Create"))
    {
        if (ImGui::Button("New Object"))
        {
            ImGui::OpenPopup("New Object");
        }

        if (ImGui::BeginPopupModal("New Object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::InputText("##", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue) || SameLineButton("Create"))
            {
                GameObjectManager::GetInstance().CreateObject(buffer);
                ImGui::CloseCurrentPopup();
                ClearBuffer();
            }

            ImGui::EndPopup();
        }

        if (ImGui::Button("Load Prefab"))
        {
            ImGui::OpenPopup("Load Prefab");
        }

        if (ImGui::BeginPopupModal("Load Prefab", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::InputText("Name", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue);
            static std::string s;

            FileSelectCombo(s, "Prefab", s, "Assets/Prefab", ".prefab");
           
            if (ImGui::Button("Load"))
            {
                Prefab::NewGameObject(buffer, s);
                ImGui::CloseCurrentPopup();
                ClearBuffer();
            }
            
            ImGui::EndPopup();
        }

        ImGui::TreePop();
    }
}

void Editor::SelectedGameObjectInfo()
{
    if (selected == nullptr)
        return;

    ImGui::Begin("Current Object");

    ImGui::BulletText(("Name: " + selected->GetName()).c_str());

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
            if (!it.second->Edit())
                break;
        }

        ImGui::TreePop();
    }

    if (ImGui::InputText("##Name", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue) || SameLineButton("Rename"))
    {
        GameObjectManager::GetInstance().RenameObject(selected->GetName(), buffer);
        ClearBuffer();
    }

    if (ImGui::Button("Save Prefab"))
    {
        ImGui::OpenPopup("Save Prefab");
    }
     
    if (ImGui::BeginPopupModal("Save Prefab", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::InputText("##Name", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue) || SameLineButton("Save"))
        {
            Prefab::SavePrefab(buffer, selected);   
            ImGui::CloseCurrentPopup();
            ClearBuffer();
        }

        ImGui::EndPopup();
    }

    if (ImGui::Button("Delete Object"))
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

    ModeChangeWindow();

    if (mode != PLAY)
    {
        ImGui::ShowDemoWindow(); // Show demo window! :)

        TopBar();
        GameObjectInfo();
        SelectedGameObjectInfo();

        Camera::GetInstance().Info();
    }
    
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
