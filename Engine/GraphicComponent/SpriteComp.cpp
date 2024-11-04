#include "SpriteComp.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../ResourceManager/ResourceManager.h"
#include "../LayerManager/LayerManager.h"
#include "../Camera/Camera.h"

#include "../Editor/Util.h"
#include "../EngineComponent/TransformComp.h"
#include "../EngineComponent/ColliderComp.h"

#include "../Profiler/Profiler.h"

SpriteComp::SpriteComp(GameObject* _owner) : GraphicComponent(_owner), 
                                             color{ 1.f, 1.f, 1.f, 1.f }, texobj(nullptr)
{
    SetShdrpgm("base.shd");
    SetMesh("square.msh");
    SetTexture("base.png");
    SetDepth();

    lineTexobj = ResourceManager::GetInstance().GetResourcePointer<GLuint>("base.png");
}

SpriteComp::~SpriteComp()
{
    UnsetShdrpgm();
    UnsetMesh();
    UnsetTexture();
}

void SpriteComp::Update()
{
    DEBUG_PROFILER_START("SpriteComp");

    glUseProgram(*shaderProgram);

    DrawSprite();

    if (owner->selected)
    {
        DrawOutline();
    }

    if (Editor::GetInstance().GetViewColliderLine() && Editor::GetInstance().GetMode() == Editor::PLAY)
    {
        DrawColliderLine();
    }

    glUseProgram(0);

    DEBUG_PROFILER_END;
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

        if (DeleteCompButton<SpriteComp>())
            return false;

        ImGui::TreePop();
    }

    if (DeleteCompMenuItem<SpriteComp>())
        return false;

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

    glBindTextureUnit(6, *lineTexobj);
    glBindTexture(GL_TEXTURE_2D, *lineTexobj);
    glBindVertexArray(mesh->VAO);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDrawArrays(GL_LINE_LOOP, 0, mesh->draw_cnt);
}

void SpriteComp::DrawColliderLine()
{
    ColliderComp* c = owner->GetComponent<ColliderComp>();
    if (!c) return;

    GLuint loc = glGetUniformLocation(*shaderProgram, "uColor");
    if (loc >= 0)
    {
        const float* colliderLineColor = Editor::GetInstance().GetColliderLineColor();
        glUniform4fv(loc, 1, colliderLineColor);
    }

    loc = glGetUniformLocation(*shaderProgram, "uModel_to_NDC");
    if (loc >= 0)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance().GetMatrix() * c->GetMatrix()));
    }

    loc = glGetUniformLocation(*shaderProgram, "uCoordZ");
    if (loc >= 0)
    {
        glUniform1f(loc, 0);
    }

    glBindTextureUnit(6, *lineTexobj);
    glBindTexture(GL_TEXTURE_2D, *lineTexobj);
    glBindVertexArray(mesh->VAO);
    glBindTexture(GL_TEXTURE_2D, 0);

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

void SpriteComp::SetDepth()
{
    depth = LayerManager::GetInstance().GetDepth(owner->GetLayerName());
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
        for (int i = 0; i < 4; i++)
        {
            color[i] = (it->begin() + i).value();
        }

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
    return _owner->AddComponent<SpriteComp>();
}
