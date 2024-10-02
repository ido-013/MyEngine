#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../OpenGL/GLSLShader.h"
#include "GraphicComponent.h"

class SpriteComp : public GraphicComponent
{
public:
	struct Color
	{
		unsigned char r = 255;
		unsigned char g = 255;
		unsigned char b = 255;
	};

private:
	Color color;

	// 각각 다른 shader를 사용할 수 있도록 수정하기
	GLuint shaderProgram;
	GLuint VBO, VAO;

	// resourceManager
	GLuint texobj;

	GLenum primitive_type;
	GLuint draw_cnt;

	std::string textureName;

public:
	SpriteComp(GameObject* _owner);
	~SpriteComp();

	// Draw
	void Update() override;

	// Gettors/Settors
	Color& GetColor() { return color; }

	void SetColor(const unsigned char& r, const unsigned char& g, const unsigned char& b);
	void SetTexture(std::string s);

	void SetupShdrpgm();
	void SetupVAO();
	void SetupTexobj();

	static constexpr const char* TypeName = "SpriteComp";
};
