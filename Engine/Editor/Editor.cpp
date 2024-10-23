#include "Editor.h"

#include "../OpenGL/GLHelper.h"

#include "../GameObjectManager/GameObjectManager.h"
#include "../Serializer/Serializer.h"
#include "../RTTI/Registry.h"
#include "../Camera/Camera.h"
#include "../Prefab/Prefab.h"

#include "Util.h"
#include "../collisionManager/CollisionUtil.h"

#include "../Components.h"

Editor::Editor() : selectedObj(nullptr), mode(EDIT), isDrag(false), mouseOffset()
{
    comps =
    {
        TransformComp::TypeName,
        SpriteComp::TypeName,
        RigidbodyComp::TypeName,
        PlayerComp::TypeName,
        ColliderComp::TypeName,
        AttackComp::TypeName,
    };
}

Editor::~Editor()
{

}

void Editor::ChangeSelectedObject(GameObject* _obj)
{
    if (selectedObj != nullptr)
    {
        selectedObj->selected = false;
    }
    
    if (_obj != nullptr)
    {
        _obj->selected = true;
    }

    selectedObj = _obj;
}

void Editor::UpdateTfComps()
{
    for (auto& tf : tfComps)
    {
        tf->CalculateMatrix();
    }
}

void Editor::ObjectMouseInteraction()
{
    ObjectPick();
    ObjectDrag();
}

void Editor::ObjectPick()
{
    if (isDrag || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
        return;

    GameObject* obj = nullptr;

    if (GLHelper::mousestateLeft)
    {
        for (auto& tf : tfComps)
        {
            if (IsCollisionPointSquare(GLHelper::mousePos, tf->GetPos(), tf->GetScale()))
            {
                obj = tf->GetOwner();
                if (selectedObj != nullptr && selectedObj == obj) break;
            }
        }

        ChangeSelectedObject(obj);

        if (obj != nullptr)
        {
            isDrag = true;
            mouseOffset = obj->GetComponent<TransformComp>()->GetPos() - GLHelper::mousePos;
        }
    }
}

void Editor::ObjectDrag()
{
    if (isDrag)
    {
        selectedObj->GetComponent<TransformComp>()->SetPos(GLHelper::mousePos + mouseOffset);
    }

    if (!GLHelper::mousestateLeft)
    {
        isDrag = false;
    }
}

void Editor::ChangeModeWindow()
{
    ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->GetCenter().x, 30 }, ImGuiCond_Always, {0.5, 0.0});
    ImGui::SetNextWindowSize({ 100, 30 });

    ImGui::Begin("mode", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

    ImGui::Columns(2, 0, false);

    if (ImGui::Selectable("Play", mode == PLAY) && mode == EDIT)
    {
        ImGui::SetWindowFocus(NULL);
        Serializer::GetInstance().SaveLevel("temp");
        ChangeSelectedObject(nullptr);
        mode = PLAY;
    }

    ImGui::NextColumn();
    if (ImGui::Selectable("Edit", mode == EDIT) && mode == PLAY)
    {
        GameObjectManager::GetInstance().RemoveAllObject();
        Serializer::GetInstance().LoadLevel("temp.lvl");
        Serializer::GetInstance().DeleteLevel("temp");
        mode = EDIT;
    }

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
        ChangeSelectedObject(nullptr);
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
            ChangeSelectedObject(nullptr);
            GameObjectManager::GetInstance().RemoveAllObject();
            Serializer::GetInstance().LoadLevel(buffer);
            buffer.clear();
            ImGui::CloseCurrentPopup();
        }

        ClosePopupButton();

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
            ImGui::CloseCurrentPopup();
        }

        ClosePopupButton();

        ImGui::EndPopup();
    }
}

void Editor::GameObjectWindow()
{
    ImGui::Begin("GameObject");

    ObjectListTree();
    ImGui::Separator();

    CreateObjectPopup();
    LoadPrefabPopup();

    ImGui::End();
}

void Editor::ObjectListTree()
{
    if (ImGui::TreeNode("List"))
    {
        for (auto& it : GameObjectManager::GetInstance().GetAllObject())
        {
            // Can cause performance degradation
            if (ImGui::MenuItem(it.first.c_str(), 0, selectedObj == nullptr ? false : !selectedObj->GetName().compare(it.first)))
            {
                ChangeSelectedObject(it.second);
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

        ClosePopupButton();

        ImGui::EndPopup();
    }
}

void Editor::LoadPrefabPopup()
{
    if (SameLineButton("Load Prefab"))
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

        ClosePopupButton();

        ImGui::EndPopup();
    }
}

void Editor::SelectedGameObjectWindow()
{
    if (selectedObj == nullptr)
        return;

    ImGui::Begin("Selected GameObject", 0, ImGuiWindowFlags_NoFocusOnAppearing);

    GameObjectInfoText();
    ImGui::Separator();

    AddComponentTree();
    ImGui::Separator();

    ComponentListTree();
    ImGui::Separator();

    RenameObjectPopup();
    SavePrefabPopup();
    DeleteObjectButton();

    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
        ChangeSelectedObject(nullptr);
    }

    ImGui::End();
}

void Editor::GameObjectInfoText()
{
    ImGui::BulletText(("Name: " + selectedObj->GetName()).c_str());
    
    std::string prefabName = selectedObj->GetPrefabName();
    if (!prefabName.empty())
        ImGui::BulletText(("Prefab: " + prefabName.substr(0, prefabName.find_last_of('.'))).c_str());
}

void Editor::AddComponentTree()
{
    if (ImGui::TreeNode("Add Component"))
    {
        for (auto& compType : comps)
        {
            if (selectedObj->IsHaveComponent(compType))
                continue;
            
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

void Editor::RenameObjectPopup()
{
    if (ImGui::Button("Rename"))
    {
        ImGui::OpenPopup("Rename");
    }

    if (ImGui::BeginPopupModal("Rename", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        char buffer[100] = { '\0' };
        if (ImGui::InputText("Name", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            GameObjectManager::GetInstance().RenameObject(selectedObj->GetName(), buffer);
            ImGui::CloseCurrentPopup();
        }

        ClosePopupButton();

        ImGui::EndPopup();
    }
}

void Editor::SavePrefabPopup()
{
    if (SameLineButton("Save as Prefab"))
    {
        ImGui::OpenPopup("Save as Prefab");
    }

    if (ImGui::BeginPopupModal("Save as Prefab", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        char buffer[100] = { '\0' };
        if (ImGui::InputText("Name", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            Prefab::SavePrefab(buffer, selectedObj);
            ImGui::CloseCurrentPopup();
        }

        ClosePopupButton();

        ImGui::EndPopup();
    }
}

void Editor::DeleteObjectButton()
{
    if (SameLineButton("Delete Object"))
    {
        GameObjectManager::GetInstance().RemoveObject(selectedObj->GetName());
        ChangeSelectedObject(nullptr);
    }
}

void Editor::AddTfComp(TransformComp* _tf)
{
    tfComps.push_back(_tf);
}

void Editor::DeleteTfComp(const TransformComp* _tf)
{
    for (auto it = tfComps.begin(); it != tfComps.end(); it++)
    {
        if (*it == _tf)
        {
            tfComps.erase(it);
            break;
        }
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

    if (mode == EDIT)
    {
        //ImGui::ShowDemoWindow(); // Show demo window! :)

        UpdateTfComps();

        ObjectMouseInteraction();
       
        TopBar();
        GameObjectWindow();
        SelectedGameObjectWindow();

        Camera::GetInstance().Edit();
    }

    ChangeModeWindow();
    
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Exit()
{
    if (mode == PLAY)
    {
        Serializer::GetInstance().DeleteLevel("temp");
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
