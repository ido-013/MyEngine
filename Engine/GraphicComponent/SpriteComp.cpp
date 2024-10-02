#include "SpriteComp.h"

#include <iostream>
#include <fstream>
#include <array>
#include <glm/gtc/type_ptr.hpp>

#include "../EngineComponent/TransformComp.h"

const char* my_vs = {
  #include "../OpenGL/my.vert"
};

const char* my_fs = {
  #include "../OpenGL/my.frag"
};

SpriteComp::SpriteComp(GameObject* _owner) : GraphicComponent(_owner), color(), textureName("../Assets/duck-rgba-256.tex"), alpha(1.f)
{
    SetupShdrpgm();
    SetupVAO();
    SetupTexobj();
}

SpriteComp::~SpriteComp()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void SpriteComp::Update()
{
    TransformComp* t = owner->GetComponent<TransformComp>();
    if (!t) return;

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    GLint loc = glGetUniformLocation(shaderProgram, "uTex2d");
    if (loc >= 0) {
        glUniform1i(loc, 6);
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

    glBindVertexArray(0);
    glUseProgram(0);
}

void SpriteComp::SetColor(const unsigned char& r, const unsigned char& g, const unsigned char& b)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

void SpriteComp::SetAlpha(float _alpha)
{
	alpha = _alpha;
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
        1.0f, -1.0f, 	1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
        1.0f,  1.0f, 	1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
       -1.0f, -1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
       -1.0f,  1.0f, 	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
    };

    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glCreateBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glNamedBufferStorage(VBO, sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*)(5 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    primitive_type = GL_TRIANGLE_STRIP;
    draw_cnt = sizeof(vertices) / sizeof(GL_FLOAT) / 7;
}

void SpriteComp::SetupTexobj()
{
    GLuint width{ 256 }, height{ 256 }, bytes_per_texel{ 4 };

    std::ifstream input(textureName, std::ios::binary);
    GLchar* ptr_texels = new GLchar[width * height * bytes_per_texel];

    if (input) {
        input.read((GLchar*)ptr_texels, width * height * bytes_per_texel);
        input.close();
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &texobj);

    glTextureStorage2D(texobj, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(texobj, 0, 0, 0, width, height,
        GL_RGBA, GL_UNSIGNED_BYTE, ptr_texels);

    delete[] ptr_texels;
}
