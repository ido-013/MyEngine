#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GraphicComponent.h"
#include "../OpenGL/GLModel.h"

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

	GLuint* shaderProgram;
	std::string shaderName;

	GLModel* mesh;
	std::string meshName;

	GLuint* texobj;
	std::string textureName;

public:
	SpriteComp(GameObject* _owner);
	~SpriteComp();

	// Draw
	void Update() override;
	bool Edit() override;

	// Gettors/Settors
	Color& GetColor() { return color; }
	void SetColor(const unsigned char& r, const unsigned char& g, const unsigned char& b);

	void SetShdrpgm(const std::string& name);
	void SetMesh(const std::string& name);
	void SetTexture(const std::string& name);

	void UnsetShdrpgm();
	void UnsetMesh();
	void UnsetTexture();

	float& GetAlpha() { return alpha; }
	void SetAlpha(const float& _alpha) { alpha = _alpha; };

	void LoadFromJson(const json&) override;
	json SaveToJson() override;

	static BaseRTTI* CreateSpriteComponent(GameObject* owner);

	static constexpr const char* TypeName = "SpriteComp";
};
