#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
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
	GLfloat alpha;

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

	void SetTexture(const std::string& s);

	float& GetAlpha() { return alpha; }
	void SetAlpha(const float& _alpha) { alpha = _alpha; };

	void SetupShdrpgm();
	void SetupVAO();
	void SetupTexobj();

	void LoadFromJson(const json&) override;
	json SaveToJson() override;

	static BaseRTTI* CreateSpriteComponent(GameObject* owner);

	static constexpr const char* TypeName = "SpriteComp";
};
