#include "Editor.h"

#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"
#include "../Imgui/imgui_stdlib.h"

#include "../OpenGL/GLHelper.h"

#include "../GameObjectManager/GameObjectManager.h"
#include "../Serializer/Serializer.h"
#include "../RTTI/Registry.h"
#include "../Camera/Camera.h"
#include "../Prefab/Prefab.h"

#include "Util.h"

#include "../Components.h"

Editor::Editor() : selectedObj(nullptr), mode(EDIT)
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

    LevelMenu();

    ImGui::EndMainMenuBar();
}

void Editor::LevelMenu()
{
    static bool loadPopup = false;
    static bool savePopup = false;

    if (ImGui::BeginMenu("Level"))
    {
        NewLevelMenu();
        LoadLevelMenu(loadPopup);
        SaveLevelMenu(savePopup);

        ImGui::EndMenu();
    }

    LoadLevelPopup(loadPopup);
    SaveLevelPopup(savePopup);
}

void Editor::NewLevelMenu()
{
    if (ImGui::MenuItem("New Level"))
    {
        selectedObj = nullptr;
        GameObjectManager::GetInstance().RemoveAllObject();
    }
}

void Editor::LoadLevelMenu(bool& _popup)
{
    if (ImGui::MenuItem("Load Level"))
    {
        _popup = true;
    }
}

void Editor::LoadLevelPopup(bool& _popup)
{
    if (_popup)
    {
        ImGui::OpenPopup("Load Level");
        _popup = false;
    }

    if (ImGui::BeginPopupModal("Load Level", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static std::string buffer;
        if (FileSelectComboOnce(buffer, "Level", buffer, "Assets/Level", ".lvl"))
        {
            selectedObj = nullptr;
            GameObjectManager::GetInstance().RemoveAllObject();
            Serializer::GetInstance().LoadLevel(buffer);
            buffer.clear();
        }

        ImGui::EndPopup();
    }
    
}

void Editor::SaveLevelMenu(bool& _popup)
{
    if (ImGui::MenuItem("Save Level"))
    {
        _popup = true;
    }
}

void Editor::SaveLevelPopup(bool& _popup)
{
    if (_popup)
    {
        ImGui::OpenPopup("Save Level");
        _popup = false;
    }

    if (ImGui::BeginPopupModal("Save Level", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        char buffer[100] = { '\0' };
        if (ImGui::InputText("Name", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            Serializer::GetInstance().SaveLevel(buffer);
        }

        ImGui::EndPopup();
    }
}

void Editor::GameObjectWindow()
{
    ImGui::Begin("GameObject");

    ObjectListTree();
    MakeObjectTree();

    ImGui::End();
}

void Editor::ObjectListTree()
{
    if (ImGui::TreeNode("List"))
    {
        for (auto& it : GameObjectManager::GetInstance().GetAllObject())
        {
            if (ImGui::Button(it.first.c_str()))
            {
                selectedObj = it.second;
            }
        }

        ImGui::TreePop();
    }
}

void Editor::MakeObjectTree()
{
    if (ImGui::TreeNode("Make"))
    {
        CreateObjectPopup();
        LoadPrefabPopup();

        ImGui::TreePop();
    }
}

void Editor::CreateObjectPopup()
{
    if (ImGui::Button("New Object"))
    {
        ImGui::OpenPopup("New Object");
    }

    if (ImGui::BeginPopupModal("New Object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        char buffer[100] = { '\0' };
        if (ImGui::InputText("Name", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            GameObjectManager::GetInstance().CreateObject(buffer);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void Editor::LoadPrefabPopup()
{
    if (ImGui::Button("Load Prefab"))
    {
        ImGui::OpenPopup("Load Prefab");
    }

    if (ImGui::BeginPopupModal("Load Prefab", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char buffer[100] = { '\0' };
        ImGui::InputText("Name", buffer, 100);

        static std::string prefabName;
        FileSelectCombo(prefabName, "Prefab", prefabName, "Assets/Prefab", ".prefab");

        if (ImGui::Button("Load"))
        {
            Prefab::NewGameObject(buffer, prefabName);
            ClearBuffer(buffer, 100);
            prefabName.clear();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void Editor::SelectedGameObjectWindow()
{
    if (selectedObj == nullptr)
        return;

    ImGui::Begin("Selected GameObject");

    GameObjectInfoText();
    AddComponentTree();
    ComponentListTree();
    RenameObjectInput();
    SavePrefabPopup();
    DeleteObjectButton();

    ImGui::End();
}

void Editor::GameObjectInfoText()
{
    ImGui::BulletText(("Name: " + selectedObj->GetName()).c_str());
}

void Editor::AddComponentTree()
{
    if (ImGui::TreeNode("Add Component"))
    {
        for (auto& compType : comps)
        {
            if (ImGui::Button(compType.c_str()))
            {
                Registry::GetInstance().FindAndCreate(compType, selectedObj);
            }
        }

        ImGui::TreePop();
    }
}

void Editor::ComponentListTree()
{
    if (ImGui::TreeNode("Component List"))
    {
        for (auto& it : selectedObj->GetAllComponent())
        {
            if (!it.second->Edit())
                break;
        }

        ImGui::TreePop();
    }
}

void Editor::RenameObjectInput()
{
    char buffer[100] = { '\0' };
    if (ImGui::InputText("Name", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        GameObjectManager::GetInstance().RenameObject(selectedObj->GetName(), buffer);
    }
}

void Editor::SavePrefabPopup()
{
    if (ImGui::Button("Save Prefab"))
    {
        ImGui::OpenPopup("Save Prefab");
    }

    if (ImGui::BeginPopupModal("Save Prefab", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        char buffer[100] = { '\0' };
        if (ImGui::InputText("Name", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            Prefab::SavePrefab(buffer, selectedObj);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void Editor::DeleteObjectButton()
{
    if (ImGui::Button("Delete Object"))
    {
        GameObjectManager::GetInstance().RemoveObject(selectedObj->GetName());
        selectedObj = nullptr;
    }
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
        GameObjectWindow();
        SelectedGameObjectWindow();

        Camera::GetInstance().Window();
    }
    
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Exit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
