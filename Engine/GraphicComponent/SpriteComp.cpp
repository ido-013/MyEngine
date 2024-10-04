#include "SpriteComp.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

#include "../EngineComponent/TransformComp.h"

const char* my_vs = {
  #include "../OpenGL/my.vert"
};

const char* my_fs = {
  #include "../OpenGL/my.frag"
};

SpriteComp::SpriteComp(GameObject* _owner) : GraphicComponent(_owner), color(),  textureName("../Assets/GSwftl7boAADleI.jfif") //textureName("../Assets/duck-rgba-256.tex")
{
    SetupShdrpgm();
    SetupVAO();
    SetupTexobj();

    glUseProgram(shaderProgram);
}

SpriteComp::~SpriteComp()
{
    glUseProgram(0);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void SpriteComp::Update()
{
    TransformComp* t = owner->GetComponent<TransformComp>();
    if (!t) return;

    GLint loc = glGetUniformLocation(shaderProgram, "uTex2d");
    if (loc >= 0) {
        glUniform1i(loc, 6);
    }

    loc = glGetUniformLocation(shaderProgram, "uColor");
    if (loc >= 0) {
        glUniform3f(loc, color.r / 255.f, color.g / 255.f, color.b / 255.f);
    }

    loc = glGetUniformLocation(shaderProgram, "uModel_to_NDC");
    if (loc >= 0) {
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(t->GetMatrix()));
    }

    glBindTextureUnit(6, texobj);
    glBindTexture(GL_TEXTURE_2D, texobj);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDrawArrays(primitive_type, 0, draw_cnt);
}

void SpriteComp::SetColor(const unsigned char& r, const unsigned char& g, const unsigned char& b)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

void SpriteComp::SetTexture(std::string name)
{
	textureName = name;
}

void SpriteComp::SetupShdrpgm()
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &my_vs, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &my_fs, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void SpriteComp::SetupVAO()
{
    GLfloat vertices[] = {
        1.0f, -1.0f, 	1.0f, 0.0f,
        1.0f,  1.0f, 	1.0f, 1.0f,
       -1.0f, -1.0f,	0.0f, 0.0f,
       -1.0f,  1.0f, 	0.0f, 1.0f,
    };

    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glCreateBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glNamedBufferStorage(VBO, sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    primitive_type = GL_TRIANGLE_STRIP;
    draw_cnt = sizeof(vertices) / sizeof(GL_FLOAT) / 4;
}

void SpriteComp::SetupTexobj()
{
    GLint width, height, bytes_per_texel;
    stbi_set_flip_vertically_on_load(1);    

    GLubyte* ptr_texels = stbi_load(textureName.c_str(), &width, &height, &bytes_per_texel, 4);

    glCreateTextures(GL_TEXTURE_2D, 1, &texobj);

    glTextureStorage2D(texobj, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(texobj, 0, 0, 0, width, height,
        GL_RGBA, GL_UNSIGNED_BYTE, ptr_texels);

    stbi_image_free(ptr_texels);
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
