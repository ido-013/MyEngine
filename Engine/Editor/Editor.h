#pragma once

#include <iostream>
#include <vector>
#include <list>
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

    std::map<std::string, bool> isSaveLevelPrefabComp;

    void ChangeSelectedObject(GameObject* _obj);
    void UpdateTfComps();

    bool isDrag;
    glm::vec2 mouseOffset;

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
            void AddLayerMenuItem(bool& _popup);
            void AddLayerPopup(bool& _popup);
        void AddComponentTree();
        void ComponentListTree();
        void RenameObjectPopup();
        void SavePrefabPopup();
        void DeleteObjectButton();

    void UtilsWindow();
        void OutlineColorTree();
        void PrefabCompTree();

    //extra
        //function key
            //ctrl + '' -> use stack

public:
    static Editor& GetInstance()
    {
        static Editor instance;
        return instance;
    }

    const EditorMode& GetMode() const { return mode; }
    const float* GetOutlineColor() const { return outlineColor; }

    void AddTfComp(TransformComp* _tf);
    void DeleteTfComp(const TransformComp* _tf);

    void Init();
    void Update();
    void Exit();
};
