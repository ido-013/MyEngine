#include "SpriteComp.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../ResourceManager/ResourceManager.h"
#include "../Camera/Camera.h"

#include "../Editor/Util.h"
#include "../EngineComponent/TransformComp.h"

SpriteComp::SpriteComp(GameObject* _owner) : GraphicComponent(_owner), 
                                             color{ 1.f, 1.f, 1.f, 1.f }, texobj(nullptr), depth(1)
{
    SetShdrpgm("base.shd");
    SetMesh("square.msh");
    SetTexture("base.png");
}

SpriteComp::~SpriteComp()
{
    UnsetShdrpgm();
    UnsetMesh();
    UnsetTexture();
}

void SpriteComp::Update()
{
    glUseProgram(*shaderProgram);

    DrawSprite();

    if (owner->selected)
    {
        DrawOutline();
    }

    glUseProgram(0);
}

bool SpriteComp::Edit()
{
    if (ImGui::TreeNode(TypeName))
    {
        //Edit Color
        ImGui::ColorEdit4("Color", color);

        ImGui::InputFloat("Depth", &depth);

        std::string filename;

        //Edit Shader
        if (FileSelectComboOnce(filename, "Shader", shaderName, "Assets/Shader", ".shd"))
        {
            SetShdrpgm(filename);
        }
        
        //Edit Mesh
        if (FileSelectComboOnce(filename, "Mesh", meshName, "Assets/Mesh", ".msh"))
        {
            SetMesh(filename);
        }
        
        //Edit Texture
        if (FileSelectComboOnce(filename, "Texture", textureName, "Assets/Texture"))
        {
            SetTexture(filename);
        }

        if (ImGui::Button("Delete Component"))
        {
            owner->DeleteComponent<SpriteComp>();
            ImGui::TreePop();
            return false;
        }

        ImGui::TreePop();
    }

    return true;
}

void SpriteComp::DrawSprite()
{
    TransformComp* t = owner->GetComponent<TransformComp>();
    if (!t) return;

    GLint loc = glGetUniformLocation(*shaderProgram, "uTex2d");
    if (loc >= 0)
    {
        glUniform1i(loc, 6);
    }

    loc = glGetUniformLocation(*shaderProgram, "uColor");
    if (loc >= 0)
    {
        glUniform4fv(loc, 1, color);
    }

    loc = glGetUniformLocation(*shaderProgram, "uModel_to_NDC");
    if (loc >= 0)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance().GetMatrix() * t->GetMatrix()));
    }

    loc = glGetUniformLocation(*shaderProgram, "uCoordZ");
    if (loc >= 0)
    {
        glUniform1f(loc, depth);
    }

    glBindTextureUnit(6, *texobj);
    glBindTexture(GL_TEXTURE_2D, *texobj);

    glBindVertexArray(mesh->VAO);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDrawArrays(mesh->primitive_type, 0, mesh->draw_cnt);
}

void SpriteComp::DrawOutline()
{
    GLuint loc = glGetUniformLocation(*shaderProgram, "uColor");
    if (loc >= 0)
    {
        const float* outlineColor = Editor::GetInstance().GetOutlineColor();
        glUniform4fv(loc, 1, outlineColor);
    }

    loc = glGetUniformLocation(*shaderProgram, "uCoordZ");
    if (loc >= 0)
    {
        glUniform1f(loc, 0);
    }

    glDrawArrays(GL_LINE_LOOP, 0, mesh->draw_cnt);
}

void SpriteComp::SetColor(const float& _r, const float& _g, const float& _b)
{
	color[0] = _r;
	color[1] = _g;
	color[2] = _b;
}

void SpriteComp::SetShdrpgm(const std::string& _name)
{
    if (shaderName.compare(_name) == 0)
        return;

    if (shaderProgram)
        UnsetShdrpgm();

    shaderName = _name;
    shaderProgram = ResourceManager::GetInstance().GetResourcePointer<GLuint>(_name);
}

void SpriteComp::SetMesh(const std::string& _name)
{
    if (meshName.compare(_name) == 0)
        return;

    if (mesh)
        UnsetMesh();

    meshName = _name;
    mesh = ResourceManager::GetInstance().GetResourcePointer<GLModel>(_name);
}

void SpriteComp::SetTexture(const std::string& _name)
{
    if (textureName.compare(_name) == 0)
        return;

    if (texobj)
        UnsetTexture();

    textureName = _name;
    texobj = ResourceManager::GetInstance().GetResourcePointer<GLuint>(_name);
}

void SpriteComp::UnsetShdrpgm()
{
    ResourceManager::GetInstance().UnloadResource(shaderName);
}

void SpriteComp::UnsetMesh()
{
    ResourceManager::GetInstance().UnloadResource(meshName);
}

void SpriteComp::UnsetTexture()
{
    ResourceManager::GetInstance().UnloadResource(textureName);
}

void SpriteComp::LoadFromJson(const json& _data)
{
    auto compData = _data.find("compData");

    if (compData != _data.end())
    {
        auto it = compData->find("color");
        color[0] = it->begin().value();

        it = compData->find("shaderName");
        SetShdrpgm(it.value());

        it = compData->find("meshName");
        SetMesh(it.value());

        it = compData->find("textureName");
        SetTexture(it.value());
    }
}

json SpriteComp::SaveToJson()
{
    json data;
    data["type"] = TypeName;

    json compData;
    compData["color"] = color;
    compData["shaderName"] = shaderName;
    compData["meshName"] = meshName;
    compData["textureName"] = textureName;

    data["compData"] = compData;

    return data;
}

BaseRTTI* SpriteComp::CreateSpriteComponent(GameObject* _owner)
{
    BaseRTTI* p = new SpriteComp(_owner);
    _owner->AddComponent<SpriteComp>(static_cast<SpriteComp*>(p));
    return p;
}
