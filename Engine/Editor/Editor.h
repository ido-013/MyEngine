#pragma once

#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"

#include <iostream>

class Editor
{
public:
    enum EditorMode
    {
        play, pause, edit
    };

private:
    static char buffer[100];
    static void TopBar();

    //ShowAllObjects() - f
        //GameObject* selected (private)

    //SelectedObjectWindow() - f
        //AddComponent
        //TODO: Move to factory
        //if(ImGui::TreeNode("Add Component"))
        //std::vector<std::string> comps = {Transform::GetName(), Sprite::GetName()};
        //for (auto& compType : comps) {if (ImGui::Button(compTyp.c_str())) { selected->CreateComponent(compType); }
        //SeleteComponent
            //Call Component Edit function (virtual bool Edit() = 0)
            //Imgui::TreeNode()
            //if (ComponentBase::Edit()) { } -> override
            //Transform->Pos
                //ImGui::InputFloat2("Pos", &pos[0]);
                //
                //float t= glm::degrees(rotation);
                //ImGui::SliderFloat("Rot", &t, -360, 360); 
                //rotation = glm::radians(t);
                //
            //Imgui::TreePop();
        // 
        //DeleteComponent
        //Add delete button to ComponentBase::Edit()
        //find / erase / delete / treePop / return false

    //object picking
    //different camera
    //function key
        //ctrl + '' -> use stack

public:
    static void Init();
    static void Update();
    static void Exit();
};
