#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GraphicComponent.h"
#include "../OpenGL/GLModel.h"
#include "../Color.h"

class SpriteComp : public GraphicComponent
{
private:
	Color color;
	GLfloat alpha;

	GLuint* shaderProgram;
	std::string shaderName;

	GLModel* mesh;
	std::string meshName;

	GLuint* texobj;
	std::string textureName;

	float depth;

public:
	SpriteComp(GameObject* _owner);
	~SpriteComp();

	// Draw
	void Update() override;
	bool Edit() override;

	void DrawSprite();
	void DrawOutline();

	// Gettors/Settors
	Color& GetColor() { return color; }
	void SetColor(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b);

	void SetShdrpgm(const std::string& _name);
	void SetMesh(const std::string& _name);
	void SetTexture(const std::string& _name);

	void UnsetShdrpgm();
	void UnsetMesh();
	void UnsetTexture();

	float& GetAlpha() { return alpha; }
	void SetAlpha(const float& _alpha) { alpha = _alpha; };

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreateSpriteComponent(GameObject* _owner);

	static constexpr const char* TypeName = "SpriteComp";
};
