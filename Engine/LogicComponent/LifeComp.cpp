#include "LifeComp.h"

#include "../GameObjectManager/GameObjectManager.h"

LifeComp::LifeComp(GameObject* _owner) : LogicComponent(_owner), life(3), maxLife(3)
{
}

LifeComp::~LifeComp()
{
}

void LifeComp::Update()
{

}

bool LifeComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		ImGui::InputInt("Max Life", &maxLife);
		maxLife = std::max(1, maxLife);

		ImGui::SliderInt("Life", &life, 1, maxLife);
		life = std::min(life, maxLife);

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
	life += _value;

	std::cout << life << std::endl; //

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
	}
}

json LifeComp::SaveToJson()
{
	json data;
	data["type"] = TypeName;

	json compData;
	compData["life"] = life;

	data["compData"] = compData;

	return data;
}

BaseRTTI* LifeComp::CreateLifeComponent(GameObject* _owner)
{
	return _owner->AddComponent<LifeComp>();
}

