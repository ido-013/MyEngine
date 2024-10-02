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
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
	};

private:
	Color color;
	GLfloat alpha;

	GLuint shaderProgram;
	GLuint VBO, VAO;
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
	void SetAlpha(float _alpha);
	void SetTexture(std::string s);

	void SetupShdrpgm();
	void SetupVAO();
	void SetupTexobj();

	static constexpr const char* TypeName = "SpriteComp";
};
