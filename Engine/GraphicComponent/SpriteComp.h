#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GraphicComponent.h"
#include "../OpenGL/GLModel.h"

class SpriteComp : public GraphicComponent
{
private:
	float color[4];

	GLuint* shaderProgram;
	std::string shaderName;

	GLModel* mesh;
	std::string meshName;

	GLuint* texobj;
	std::string textureName;

	GLuint* lineTexobj;

	float depth;

public:
	SpriteComp(GameObject* _owner);
	~SpriteComp();

	// Draw
	void Update() override;
	bool Edit() override;

	void DrawSprite();
	void DrawOutline();
	void DrawColliderLine();

	// Gettors/Settors
	void SetColor(const float& _r, const float& _g, const float& _b);

	void SetShdrpgm(const std::string& _name);
	void SetMesh(const std::string& _name);
	void SetTexture(const std::string& _name);
	void SetDepth();

	void UnsetShdrpgm();
	void UnsetMesh();
	void UnsetTexture();

	float& GetAlpha() { return color[3]; }
	void SetAlpha(const float& _alpha) { color[3] = _alpha; };

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreateSpriteComponent(GameObject* _owner);

	static constexpr const char* TypeName = "SpriteComp";
};
