#include "SpriteComp.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../ResourceManager/ResourceManager.h"
#include "../Camera/Camera.h"

#include "../Editor/Util.h"
#include "../EngineComponent/TransformComp.h"

SpriteComp::SpriteComp(GameObject* _owner) : GraphicComponent(_owner), 
                                             color({255, 255, 255}), alpha(1), texobj(nullptr)
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
    TransformComp* t = owner->GetComponent<TransformComp>();
    if (!t) return;

    glUseProgram(*shaderProgram);

    GLint loc = glGetUniformLocation(*shaderProgram, "uTex2d");
    if (loc >= 0) 
    {
        glUniform1i(loc, 6);
    }

    loc = glGetUniformLocation(*shaderProgram, "uColor");
    if (loc >= 0) 
    {
        glUniform4f(loc, color.r / 255.f, color.g / 255.f, color.b / 255.f, alpha);
    }

    loc = glGetUniformLocation(*shaderProgram, "uModel_to_NDC");
    if (loc >= 0) 
    {
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance().GetMatrix() * t->GetMatrix()));
    }

    glBindTextureUnit(6, *texobj);
    glBindTexture(GL_TEXTURE_2D, *texobj);

    glBindVertexArray(mesh->VAO);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDrawElements(mesh->primitive_type, mesh->draw_cnt, GL_UNSIGNED_SHORT, NULL);

    glUseProgram(0);
}

bool SpriteComp::Edit()
{
    if (ImGui::TreeNode(TypeName))
    {
        //Edit Color
        int colorArray[3] = { color.r, color.g, color.b };
        ImGui::InputInt3("Color", &colorArray[0]);
        color.r = std::max(std::min(colorArray[0], 255), 0);
        color.g = std::max(std::min(colorArray[1], 255), 0);
        color.b = std::max(std::min(colorArray[2], 255), 0);

        //Edit Alpha
        ImGui::InputFloat("Alpha", &alpha);
        alpha = alpha < 0 ? 0 : (alpha > 1 ? 1 : alpha);

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

void SpriteComp::SetColor(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b)
{
	color.r = _r;
	color.g = _g;
	color.b = _b;
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
        color.r = it->begin().value();
        color.g = (it->begin() + 1).value();
        color.b = (it->begin() + 2).value();

        it = compData->find("alpha");
        alpha = it.value();

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
    compData["color"] = { color.r, color.g, color.b };
    compData["alpha"] = alpha;
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
