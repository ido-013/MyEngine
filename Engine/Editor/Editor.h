#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <glm/glm.hpp>

#include "../Imgui/imgui_inc.h"

#include "../GameObject/GameObject.h"

class TransformComp;

class Editor
{
public:
    enum EditorMode
    {
        PLAY,
        EDIT,
    };

private:
    Editor();

    Editor(const Editor&) = delete;
    const Editor& operator=(const Editor&) = delete;

    ~Editor();

    EditorMode mode;
    GameObject* selectedObj;
    std::vector<std::string> comps;
    std::list<TransformComp*> tfComps;
    float outlineColor[4];
    float colliderLineColor[4];

    bool viewColliderLine;
    bool viewProfiler;

    bool gridMode;
    std::string selectedPrefabNameGrid;

    std::map<std::string, bool> isSaveComp;
    std::map<std::string, bool> isSaveLevelPrefabComp;

    bool isDrag;
    glm::vec2 mouseOffset;

    std::string copyObjectName;

    void ChangeSelectedObject(GameObject* _obj);
    void UpdateTfComps();
    GameObject* GetObjectFromMouse();

    void ObjectMouseInteraction();
        void ObjectPick();
        void ObjectDrag();

    void ChangeModeWindow();

    void TopBar();
        void LevelMenu();
            void NewLevelMenu();
            void LoadLevelMenu(bool& _popup);
                void LoadLevelPopup(bool& _popup);
            void SaveLevelMenu(bool& _popup);
                void SaveLevelPopup(bool& _popup);

    void GameObjectWindow();
        void ObjectListTree();
        void MakeObjectTree();
            void CreateObjectPopup();
            void LoadPrefabPopup();

    void SelectedGameObjectWindow();
        void GameObjectInfoText();
        void SelectLayerCombo();
            void AddLayerMenuItem(bool& _popup, int& _layerInd, const int& _ind);
            void AddLayerPopup(bool& _popup, int& _layerInd);
        void AddComponentTree();
        void ComponentListTree();
        void RenameObjectPopup();
        void SavePrefabPopup();
        void DeleteObjectButton();
        void CopyObject();
        
    void PasteObject();

    void UtilsWindow();
        void OutlineColorTree();
        void ColliderLineTree();
        void PrefabCompTree();
        void ProfilerCheckbox();
        void GridCheckbox();

    void Grid();
        void GridWindow();
            void SelectPrefabCombo();
        void DrawGrid();
        void CreateObjectGrid();
        void DeleteObjectGrid();

    void ProfilerWindow();
        void fpsText();

public:
    static Editor& GetInstance()
    {
        static Editor instance;
        return instance;
    }

    const EditorMode& GetMode() const { return mode; }
    const float* GetOutlineColor() const { return outlineColor; }
    const bool GetViewColliderLine() const { return viewColliderLine; }
    const float* GetColliderLineColor() const { return colliderLineColor; }

    void AddTfComp(TransformComp* _tf);
    void DeleteTfComp(const TransformComp* _tf);

    void Init();
    void Update();
    void Exit();
};
