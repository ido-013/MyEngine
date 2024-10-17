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
        PLAY,
        EDIT,
        PAUSE,
    };

private:
    Editor();
    ~Editor();

    Editor(const Editor& other) = delete;
    const Editor& operator=(const Editor& other) = delete;

    EditorMode mode;
    char buffer[100];
    GameObject* selected;
    std::vector<std::string> comps;

    void ClearBuffer();
    bool SameLineButton(const char* label);

    void ModeChangeWindow();

    void TopBar();
    void TopBarLevel();

    void GameObjectInfo();
    void ObjectList();
    void CreateObject();

    void SelectedGameObjectInfo();

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

    const EditorMode& GetMode() const { return mode; }

    void Init();
    void Update();
    void Exit();
};
