#include "LifeComp.h"

#include "../OpenGL/GLHelper.h"

#include "../GameObjectManager/GameObjectManager.h"

#include "../GraphicComponent/SpriteComp.h"

LifeComp::LifeComp(GameObject* _owner) : LogicComponent(_owner), life(3), maxLife(3), isImmune(false), immuneTime(1.5f), timer(0)
{
}

LifeComp::~LifeComp()
{
}

void LifeComp::Update()
{
	if (isImmune)
	{
		timer += (float)GLHelper::delta_time;

		if (timer > immuneTime)
		{
			timer = 0;
			isImmune = false;
			owner->GetComponent<SpriteComp>()->SetColor(255, 255, 255);
		}
	}
}

bool LifeComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		ImGui::InputInt("Max Life", &maxLife);
		maxLife = std::max(1, maxLife);

		ImGui::SliderInt("Life", &life, 1, maxLife);
		life = std::min(life, maxLife);

		ImGui::InputFloat("Immune Time", &immuneTime);

		if (DeleteCompButton<LifeComp>())
			return false;

		ImGui::TreePop();
	}

	if (DeleteCompMenuItem<LifeComp>())
		return false;

	return true;
}

void LifeComp::AddLife(const int& _value)
{
	if (isImmune)
		return;

	if (_value < 0)
	{
		isImmune = true;

		//animator update after
		owner->GetComponent<SpriteComp>()->SetColor(100, 0, 0);
	}

	life += _value;

	if (life > maxLife)
	{
		life = maxLife;
	}

	if (life <= 0)
	{
		//death event
		GameObjectManager::GetInstance().ReservationRemoveObject(owner->GetName());
	}
}

void LifeComp::LoadFromJson(const json& _data)
{
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto it = compData->find("life");
		life = it.value();

		it = compData->find("maxLife");
		maxLife = it.value();
		
		it = compData->find("immuneTime");
		immuneTime = it.value();
	}
}

json LifeComp::SaveToJson()
{
	json data;
	data["type"] = TypeName;

	json compData;
	compData["life"] = life;
	compData["maxLife"] = maxLife;
	compData["immuneTime"] = immuneTime;

	data["compData"] = compData;

	return data;
}

BaseRTTI* LifeComp::CreateLifeComponent(GameObject* _owner)
{
	return _owner->AddComponent<LifeComp>();
}

