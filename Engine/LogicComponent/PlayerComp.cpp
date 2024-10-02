#include "PlayerComp.h"

#include <string>
#include "../OpenGL/GLHelper.h"
#include "../EngineComponent/TransformComp.h"
#include "../EngineComponent/RigidbodyComp.h"
#include "../GraphicComponent/SpriteComp.h"

PlayerComp::PlayerComp(GameObject* _owner) : LogicComponent(_owner)
{

}

PlayerComp::~PlayerComp()
{

}

void PlayerComp::Update()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	RigidbodyComp* r = owner->GetComponent<RigidbodyComp>();
	if (!r) return;

	r->ClearVelocity();

	if (GLHelper::keystateW)
	{
		r->AddVelocity(0, speed);
	}

	if (GLHelper::keystateA)
	{
		r->AddVelocity(-speed, 0);
	}

	if (GLHelper::keystateS)
	{
		r->AddVelocity(0, -speed);
	}

	if (GLHelper::keystateD)
	{
		r->AddVelocity(speed, 0);
	}
}
