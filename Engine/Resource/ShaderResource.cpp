#include "ShaderResource.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

ShaderResource::~ShaderResource()
{
    if (data)
    {
        GLuint* pShaderProgram = static_cast<GLuint*>(data);
        glDeleteProgram(*pShaderProgram);
        delete pShaderProgram;
    }

    data = nullptr;
}

void ShaderResource::LoadData(const std::string& filename)
{
    std::ifstream shaderFile;
    shaderFile.open("Assets/Shader/" + filename);

    std::string vShaderFileName;
    std::string fShaderFileName;

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();

    shaderStream >> vShaderFileName >> fShaderFileName;

    //
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.open(vShaderFileName);
    fShaderFile.open(fShaderFileName);

    if (!vShaderFile.is_open())
        exit(0);

    std::stringstream vShaderStream;
    std::stringstream fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
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
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    GLuint* pShaderProgram = new GLuint;
    *pShaderProgram = glCreateProgram();
    glAttachShader(*pShaderProgram, vertexShader);
    glAttachShader(*pShaderProgram, fragmentShader);
    glLinkProgram(*pShaderProgram);

    // check for linking errors
    glGetProgramiv(*pShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*pShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    data = pShaderProgram;
}
