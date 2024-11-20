#include "Editor.h"

#include "../OpenGL/GLHelper.h"

#include "../Imgui/implot.h"

#include "../GameObjectManager/GameObjectManager.h"
#include "../LayerManager/LayerManager.h"
#include "../ResourceManager/ResourceManager.h"
#include "../Serializer/Serializer.h"
#include "../RTTI/Registry.h"
#include "../Camera/Camera.h"
#include "../Prefab/Prefab.h"
#include "../Profiler/Profiler.h"
#include "Grid.h"
#include "../Pathfinder/Pathfinder.h"

#include "Util.h"
#include "../collisionManager/CollisionUtil.h"

#include "../Components.h"

Editor::Editor() : selectedObj(nullptr), mode(EDIT), isDrag(false), mouseOffset(), 
                   outlineColor{1.f, 1.f, 0.f, 1.f}, colliderLineColor{1.f, 0.f, 0.f, 1.f}, 
                   viewColliderLine(false), viewProfiler(false),
                   gridMode(false),
                   copyObjectName()
{
    comps =
    {
        PlayerComp::TypeName,
        LifeComp::TypeName,
        BombComp::TypeName,
        BulletComp::TypeName,
        EnemyComp::TypeName,

        TransformComp::TypeName,
        RigidbodyComp::TypeName,
        ColliderComp::TypeName,
        LifetimeComp::TypeName,

        SpriteComp::TypeName,
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

GameObject* Editor::GetObjectFromMouse()
{
    GameObject* obj = nullptr;

    for (auto& tf : tfComps)
    {
        if (IsCollisionPointSquare(GLHelper::mousePos, tf->GetPos(), tf->GetScale()))
        {
            obj = tf->GetOwner();
            if (selectedObj != nullptr && selectedObj == obj) break;
        }
    }

    return obj;
}

void Editor::ObjectMouseInteraction()
{
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
        return;

    if (gridMode)
        return;

    ObjectPick();
    ObjectDrag();
}

void Editor::ObjectPick()
{
    if (isDrag)
        return;

    if (GLHelper::mousestateLeft)
    {
        GameObject* obj = GetObjectFromMouse();

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
        Serializer::GetInstance().SaveLevel("temp", isSaveLevelPrefabComp, true);
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

        if (ClosePopupButton())
        {
            buffer.clear();
            ImGui::CloseCurrentPopup();
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
            Serializer::GetInstance().SaveLevel(buffer, isSaveLevelPrefabComp);

            ImGui::CloseCurrentPopup();
        }

        if (ClosePopupButton())
        {
            ImGui::CloseCurrentPopup();
        }

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

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Delete"))
                {
                    GameObjectManager::GetInstance().RemoveObject(it.first);
                    ChangeSelectedObject(nullptr);
                    ImGui::EndPopup();
                    break;
                }

                ImGui::EndPopup();
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
            ChangeSelectedObject(GameObjectManager::GetInstance().CreateObject(buffer));
            ImGui::CloseCurrentPopup();
        }

        if (ClosePopupButton())
        {
            ImGui::CloseCurrentPopup();
        }

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

        if (ClosePopupSameLineButton())
        {
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

    ImGui::Begin("Selected GameObject", 0, ImGuiWindowFlags_NoFocusOnAppearing);

    GameObjectInfoText();
    ImGui::Separator();

    SelectLayerCombo();
    ImGui::Separator();

    AddComponentTree();
    ImGui::Separator();

    ComponentListTree();
    ImGui::Separator();

    RenameObjectPopup();
    SavePrefabPopup();
    DeleteObjectButton();

    CopyObject();

    if (ImGui::IsWindowFocused() && GLHelper::keyState[GLFW_KEY_ESCAPE])
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
    {
        ImGui::BulletText(("Prefab: " + prefabName.substr(0, prefabName.find_last_of('.'))).c_str());
    }
}

void Editor::SelectLayerCombo()
{
    static bool isAddLayer = false;
    static int layerInd = 0;

    LayerManager& layerManager = LayerManager::GetInstance();

    if (ImGui::BeginCombo("Layer", selectedObj->GetLayerName().c_str()))
    {
        for (int i = 0; i <= LayerManager::maxLayerInd; i++)
        {
            if (layerManager.GetName(i).empty())
            {
                AddLayerMenuItem(isAddLayer, layerInd, i);
            }

            else
            {
                if (ImGui::MenuItem(((std::to_string(i) + (i < 10 ? "  " : " ") + layerManager.GetName(i)).c_str())))
                {
                    selectedObj->SetLayer(i);
                }

                if (i == 0 || i == LayerManager::maxLayerInd)
                    continue;

                if (ImGui::BeginPopupContextItem())
                {
                    if (ImGui::MenuItem("Delete"))
                    {
                        LayerManager::GetInstance().DeleteLayer(i);
                    }
                    ImGui::EndPopup();
                }
            }
        }

        ImGui::EndCombo();
    }
    
    AddLayerPopup(isAddLayer, layerInd);
}

void Editor::AddLayerMenuItem(bool& _popup, int& _layerInd, const int& _ind)
{
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

    if (ImGui::MenuItem((std::to_string(_ind) + (_ind < 10 ? "  " : " ") + "Add Layer...").c_str()))
    {
        _popup = true;
        _layerInd = _ind;
    }

    ImGui::PopStyleVar();
}

void Editor::AddLayerPopup(bool& _popup, int& _layerInd)
{
    if (_popup)
    {
        ImGui::OpenPopup("Add Layer");
        _popup = false;
    }

    if (ImGui::BeginPopupModal("Add Layer", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char buffer[100] = { '\0' };

        ImGui::InputText("Name", buffer, 100);

        if (ImGui::Button("Add Layer"))
        {
            LayerManager::GetInstance().AddLayer(_layerInd, buffer);

            ClearBuffer(buffer, 100);
            ImGui::CloseCurrentPopup();
        }

        if (ClosePopupSameLineButton())
        {
            ClearBuffer(buffer, 100);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
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

        if (ClosePopupButton())
        {
            ImGui::CloseCurrentPopup();
        }

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
        if (ImGui::BeginChild("##", ImVec2(-FLT_MIN, ImGui::GetFontSize() * 10), ImGuiChildFlags_Borders))
        {
            for (auto& compType : comps)
            {
                if (!selectedObj->IsHaveComponent(compType))
                    continue;

                ImGui::Checkbox(compType.c_str(), &isSaveComp[compType]);
            }

            ImGui::EndChild();
        }

        static char buffer[100] = { '\0' };            
        ImGui::InputText("Name", buffer, 100);

        if (ImGui::Button("Save"))
        {
            Prefab::SavePrefab(buffer, selectedObj, isSaveComp);
            ResourceManager::GetInstance().ReloadResource(std::string(buffer) + ".prefab");

            isSaveComp.clear();
            ClearBuffer(buffer, 100);
            ImGui::CloseCurrentPopup();
        }

        if (ClosePopupSameLineButton())
        {
            isSaveComp.clear();
            ClearBuffer(buffer, 100);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void Editor::DeleteObjectButton()
{
    if (SameLineButton("Delete Object") || GLHelper::keyState[GLFW_KEY_DELETE])
    {
        GameObjectManager::GetInstance().RemoveObject(selectedObj->GetName());
        ChangeSelectedObject(nullptr);
    }
}

void Editor::CopyObject()
{
    if (gridMode)
        return;

    if (GLHelper::ctrlKeyState[GLFW_KEY_C])
    {
        GLHelper::ctrlKeyState[GLFW_KEY_C] = GL_FALSE;
        copyObjectName = selectedObj->GetName();
        Prefab::SavePrefab("temp", selectedObj, isSaveComp, true);
    }
}

void Editor::PasteObject()
{
    if (gridMode)
        return;

    if (copyObjectName.empty())
        return;

    if (GLHelper::ctrlKeyState[GLFW_KEY_V])
    {
        GLHelper::ctrlKeyState[GLFW_KEY_V] = GL_FALSE;
        ChangeSelectedObject(Prefab::NewGameObject(copyObjectName, "temp.prefab"));
        
        TransformComp* t = selectedObj->GetComponent<TransformComp>();

        if (t != nullptr)
        {
            t->SetPos(GLHelper::mousePos);
        }
    }
}

void Editor::UtilsWindow()
{
    ImGui::Begin("Utils");

    Camera::GetInstance().Edit();
    OutlineColorTree();
    ColliderLineTree();
    PrefabCompTree();
    ProfilerCheckbox();
    GridCheckbox();

    ImGui::End();
}

void Editor::OutlineColorTree()
{
    if (ImGui::TreeNode("Outline Color"))
    {
        ImGui::ColorEdit4("Color", outlineColor);

        ImGui::TreePop();
    }
}

void Editor::ColliderLineTree()
{
    if (ImGui::TreeNode("Collider Line"))
    {
        ImGui::Checkbox("View Collider Line", &viewColliderLine);

        ImGui::ColorEdit4("Color", colliderLineColor);

        ImGui::TreePop();
    }
}

void Editor::PrefabCompTree()
{
    if (ImGui::TreeNode("Prefab Component to Save Level"))
    {
        for (auto& compType : comps)
        {
            ImGui::Checkbox(compType.c_str(), &isSaveLevelPrefabComp[compType]);
        }

        ImGui::TreePop();
    }
}

void Editor::ProfilerCheckbox()
{
    ImGui::Checkbox("View Profiler", &viewProfiler);
}

void Editor::GridCheckbox()
{
    ImGui::Checkbox("Grid Mode (Ctrl+G)", &gridMode);

    if (GLHelper::ctrlKeyState[GLFW_KEY_G])
    {
        GLHelper::ctrlKeyState[GLFW_KEY_G] = GL_FALSE;
        gridMode = !gridMode;
    }
}

void Editor::Grid()
{
    if (!gridMode)
        return;

    GridWindow();  
    DrawGrid();
}

void Editor::GridWindow()
{
    ImGui::Begin("Grid");

    SelectPrefabCombo();
    Grid::GetInstance().Edit();

    CreateObjectGrid();
    DeleteObjectGrid();

    ImGui::End();
}

void Editor::SelectPrefabCombo()
{
    FileSelectCombo(selectedPrefabNameGrid, "Prefab", selectedPrefabNameGrid, "Assets/Prefab", ".prefab");
}

void Editor::DrawGrid()
{
    Grid::GetInstance().Draw();
}

void Editor::CreateObjectGrid()
{
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
        return;

    if (GLHelper::mousestateLeft)
    {
        GameObject* obj = GetObjectFromMouse();

        if (obj != nullptr)
        {
            ChangeSelectedObject(obj);
        }
        else
        {
            ChangeSelectedObject(Prefab::NewGameObject(selectedPrefabNameGrid.substr(0, selectedPrefabNameGrid.find_last_of('.')), selectedPrefabNameGrid));
            
            if (selectedObj != nullptr)
            {
                TransformComp* t = selectedObj->GetComponent<TransformComp>();

                if (t != nullptr)
                {
                    int size = Grid::GetInstance().GetSize();

                    t->SetScale({ size, size });
                    t->SetPos(Grid::GetInstance().GetGridPos(GLHelper::mousePos));

                    glm::vec2 gridInd = Grid::GetInstance().GetGridInd(GLHelper::mousePos);
                    Pathfinder::GetInstance().SetTerrain((int)gridInd.x, (int)gridInd.y, selectedObj->GetLayer());
                }
            }
        }
    }
}

void Editor::DeleteObjectGrid()
{
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
        return;

    if (GLHelper::mousestateRight)
    {
        GameObject* obj = GetObjectFromMouse();

        if (obj != nullptr)
        {
            if (selectedObj == obj)
            {
                ChangeSelectedObject(nullptr);
            }

            GameObjectManager::GetInstance().RemoveObject(obj->GetName());

            glm::vec2 gridInd = Grid::GetInstance().GetGridInd(GLHelper::mousePos);
            Pathfinder::GetInstance().SetTerrain((int)gridInd.x, (int)gridInd.y);
        }
    }
}

void Editor::ProfilerWindow()
{
    if (!viewProfiler)
        return;

    ImGui::Begin("Profiler");

    fpsText();
    ImGui::Separator();

    DEBUG_PROFILER_DUMP;

    ImGui::End();
}

void Editor::fpsText()
{
    ImGui::Text("fps: %.2f", GLHelper::fps);
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
    ImPlot::CreateContext();
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
        //ImPlot::ShowDemoWindow();

        UpdateTfComps();

        ObjectMouseInteraction();
       
        TopBar();
        GameObjectWindow();
        SelectedGameObjectWindow();

        PasteObject();

        UtilsWindow();

        Grid();
    }

    else
    {
        ProfilerWindow();
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

    Prefab::DeletePrefab("temp");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}
