#include "TransformComp.h"
#include <iostream>
#include <glm/ext.hpp>

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
	transformMatrix = translateMtx * rotationMtx * scaleMtx;
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