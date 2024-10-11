#include "SpriteComp.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../ResourceManager/ResourceManager.h"
#include "../EngineComponent/TransformComp.h"

SpriteComp::SpriteComp(GameObject* _owner) : GraphicComponent(_owner), color(), textureName(), alpha(1), texobj(nullptr)
{
    shaderProgram = ResourceManager::GetInstance().GetResourcePointer<GLuint>("Assets/Shader/base.shd");
    mdl = ResourceManager::GetInstance().GetResourcePointer<GLModel>("Assets/meshes/circle.msh");
    glUseProgram(*shaderProgram);
}

SpriteComp::~SpriteComp()
{
    glUseProgram(0);
    glBindVertexArray(0);
}

void SpriteComp::Update()
{
    TransformComp* t = owner->GetComponent<TransformComp>();
    if (!t) return;

    GLint loc = glGetUniformLocation(*shaderProgram, "uTex2d");
    if (loc >= 0) {
        glUniform1i(loc, 6);
    }

    loc = glGetUniformLocation(*shaderProgram, "uColor");
    if (loc >= 0) {
        glUniform4f(loc, color.r / 255.f, color.g / 255.f, color.b / 255.f, alpha);
    }

    loc = glGetUniformLocation(*shaderProgram, "uModel_to_NDC");
    if (loc >= 0) {
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(t->GetMatrix()));
    }

    glBindTextureUnit(6, *texobj);
    glBindTexture(GL_TEXTURE_2D, *texobj);

    glBindVertexArray(mdl->VAO);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDrawElements(mdl->primitive_type, mdl->draw_cnt, GL_UNSIGNED_SHORT, NULL);
}

void SpriteComp::SetColor(const unsigned char& r, const unsigned char& g, const unsigned char& b)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

void SpriteComp::SetTexture(const std::string& name)
{
    if (texobj)
        ResourceManager::GetInstance().UnloadResource(textureName);

	textureName = name;
    texobj = ResourceManager::GetInstance().GetResourcePointer<GLuint>(name);
}

void SpriteComp::LoadFromJson(const json& data)
{
    auto compData = data.find("compData");

    if (compData != data.end())
    {
        auto it = compData->find("color");
        color.r = it->begin().value();
        color.g = (it->begin() + 1).value();
        color.b = (it->begin() + 2).value();

        it = compData->find("textureName");
        textureName = it.value();
        SetTexture(textureName);
    }
}

json SpriteComp::SaveToJson()
{
    json data;
    data["type"] = TypeName;

    json compData;
    compData["color"] = { color.r, color.g, color.b };
    compData["textureName"] = textureName;
    data["compData"] = compData;

    return data;
}

BaseRTTI* SpriteComp::CreateSpriteComponent(GameObject* owner)
{
    BaseRTTI* p = new SpriteComp(owner);
    owner->AddComponent<SpriteComp>(static_cast<SpriteComp*>(p));
    return p;
}
