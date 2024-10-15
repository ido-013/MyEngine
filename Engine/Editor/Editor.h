#pragma once

#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"

#include "../GameObject/GameObject.h"

#include <iostream>
#include <vector>

class Editor
{
public:
    enum EditorMode
    {
        play, pause, edit
    };

private:
    Editor();
    ~Editor();

    Editor(const Editor& other) = delete;
    const Editor& operator=(const Editor& other) = delete;

    char buffer[100];
    GameObject* selected;
    std::vector<std::string> comps;

    void ClearBuffer();
    bool SameLineButton(const char* label);

    void TopBar();
    void GameObjectList();
    void SelectedGameObjectInfo();

    //if (ComponentBase::Edit()) { } -> override
    //Transform->Pos
        //
        //
        //
        //
        //
        //
    //Imgui::TreePop();
    //DeleteComponent
    //Add delete button to ComponentBase::Edit()

    //extra
        //object picking
        //different camera
        //function key
            //ctrl + '' -> use stack

public:
    static Editor& GetInstance()
    {
        static Editor instance;
        return instance;
    }

    void Init();
    void Update();
    void Exit();
};
