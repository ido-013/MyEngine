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

    void TopBar();
    void GameObjectList();
    void SelectedGameObjectInfo();

            //if (ComponentBase::Edit()) { } -> override
            //Transform->Pos
                //ImGui::InputFloat2("Pos", &pos[0]);
                //
                //float t= glm::degrees(rotation);
                //ImGui::SliderFloat("Rot", &t, -360, 360); 
                //rotation = glm::radians(t);
                //
            //Imgui::TreePop();


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
