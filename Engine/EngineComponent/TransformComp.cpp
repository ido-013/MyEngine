#include "TransformComp.h"

#include <iostream>
#include <glm/ext.hpp>

#include "../Camera/Camera.h"
#include "../Editor/Editor.h"

void TransformComp::CalculateMatrix()
{
	//Create a translate matrix
	glm::mat4 translateMtx(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		pos.x, pos.y, 0.f, 1.f
	);

	//Create a rotation matrix
	float angle = glm::radians(rot);
	float c = glm::cos(angle);
	float s = glm::sin(angle);
	glm::mat4 rotationMtx(
		c, s, 0.f, 0.f,
		-s, c, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);

	//Create a scale matrix
	glm::mat4 scaleMtx(
		scale.x / 2, 0.f, 0.f, 0.f,
		0.f, scale.y / 2, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);

	//Concatenate them
	transformMatrix = translateMtx * rotationMtx * scaleMtx;
}

TransformComp::TransformComp(GameObject* _owner) : EngineComponent(_owner), pos(), scale(), rot(0), transformMatrix()
{
	pos.x = 0;
	pos.y = 0;

	scale.x = 1;
	scale.y = 1;

	CalculateMatrix();

	Editor::GetInstance().AddTfComp(this);
}

TransformComp::~TransformComp()
{
	Editor::GetInstance().DeleteTfComp(this);
}

void TransformComp::Update()
{
	CalculateMatrix();
}

bool TransformComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		ImGui::InputFloat2("Pos", &pos[0]);

		ImGui::InputFloat2("Scale", &scale[0]);

		ImGui::DragFloat("Rot", &rot, 1, 0, 360);

		if (DeleteCompButton<TransformComp>())
			return false;

		CalculateMatrix();

		ImGui::TreePop();
	}

	if (DeleteCompMenuItem<TransformComp>())
		return false;

	return true;
}

void TransformComp::SetPos(const glm::vec2& _otherPos)
{
	this->pos = _otherPos;
	CalculateMatrix();
}

void TransformComp::SetScale(const glm::vec2& _otherScale)
{
	this->scale = _otherScale;
	CalculateMatrix();
}

void TransformComp::SetRot(const float& _otherRot)
{
	this->rot = _otherRot;
	CalculateMatrix();
}

void TransformComp::PrintMatrix()
{
	std::cout << "Printing Transform Comp. With this values: " << std::endl;
	std::cout << "Translate : " << pos.x << " " << pos.y << std::endl;
	std::cout << "Rotation : " << rot << std::endl;
	std::cout << "Scale : " << scale.x << " " << scale.y << std::endl;

	for (int i = 0; i < 3; i++)
	{		
		std::cout << "| " << transformMatrix[i].x << " " << transformMatrix[i].y << " " << transformMatrix[i].z << " |\n";
	}
}

void TransformComp::LoadFromJson(const json& _data)
{
	// Check how you saved, load from there
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto p = compData->find("position");
		pos.x = p->begin().value();
		pos.y = (p->begin() + 1).value();

		auto s = compData->find("scale");
		scale.x = s->begin().value();
		scale.y = (s->begin() + 1).value();

		auto r = compData->find("rotation");
		rot = r.value();
	}
}

json TransformComp::SaveToJson()
{
	json data;

	data["type"] = TypeName;

	json compData;
	compData["position"] = { pos.x, pos.y };
	compData["scale"] = { scale.x, scale.y };
	compData["rotation"] = rot;

	data["compData"] = compData;
	return data;
}

BaseRTTI* TransformComp::CreateTransformComponent(GameObject* _owner)
{
	return _owner->AddComponent<TransformComp>();
}