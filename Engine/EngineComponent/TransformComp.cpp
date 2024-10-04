#include "TransformComp.h"

#include <iostream>
#include <glm/ext.hpp>

#include "../Camera/Camera.h"

void TransformComp::CalculateMatrix()
{
	//Create a translate matrix
	glm::mat3 translateMtx = glm::mat3(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		pos.x, pos.y, 1.f
	);

	//Create a rotation matrix
	float angle = glm::radians(rot);
	float c = glm::cos(angle);
	float s = glm::sin(angle);
	glm::mat3 rotationMtx = glm::mat3(
		c, s, 0.f,
		-s, c, 0.f,
		0.f, 0.f, 1.f
	);

	//Create a scale matrix
	glm::mat3 scaleMtx = glm::mat3(
		scale.x, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f,
		0.0f, 0.0f, 1.0f
	);

	//Concatenate them
	transformMatrix = Camera::GetInstance().GetMatrix() * translateMtx * rotationMtx * scaleMtx;
}

TransformComp::TransformComp(GameObject* _owner) : EngineComponent(_owner), pos(), scale(), rot(0), transformMatrix()
{
	pos.x = 0;
	pos.y = 0;

	scale.x = 1;
	scale.y = 1;

	CalculateMatrix();
}

TransformComp::~TransformComp()
{

}

void TransformComp::Update()
{

}

void TransformComp::SetPos(const glm::vec2& otherPos)
{
	this->pos = otherPos;

	CalculateMatrix();
}

void TransformComp::SetScale(const glm::vec2& otherScale)
{
	this->scale = otherScale;

	CalculateMatrix();
}

void TransformComp::SetRot(const float& otherRot)
{
	this->rot = otherRot;

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

void TransformComp::LoadFromJson(const json& data)
{
	// Check how you saved, load from there
	auto compData = data.find("compData");

	if (compData != data.end())
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

	// Data is loaded

	// Utilize the data
	CalculateMatrix();
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

BaseRTTI* TransformComp::CreateTransformComponent(GameObject* owner)
{
	BaseRTTI* p = new TransformComp(owner);

	owner->AddComponent<TransformComp>(static_cast<BaseComponent*>(p));

	return p;
}