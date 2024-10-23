#pragma once

#include "../GameObject/GameObject.h"

#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"
#include "../Imgui/imgui_stdlib.h"
#include "../Imgui/imgui_internal.h"

#include <iostream>
#include <vector>

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
    std::vector<TransformComp*> tfComps;

    void ChangeSelectedObject(GameObject* _obj);
    void UpdateTfComps();
    void ObjectPicking();

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
        void AddComponentTree();
        void ComponentListTree();
        void RenameObjectPopup();
        void SavePrefabPopup();
        void DeleteObjectButton();

    //extra
        //object picking
        //function key
            //ctrl + '' -> use stack

public:
    static Editor& GetInstance()
    {
        static Editor instance;
        return instance;
    }

    const EditorMode& GetMode() const { return mode; }

    void AddTfComp(TransformComp* _tf);
    void DeleteTfComp(const TransformComp* _tf);

    void Init();
    void Update();
    void Exit();
};
