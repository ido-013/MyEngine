#include "LifetimeComp.h"

#include "../OpenGL/GLHelper.h"

#include "../GameObjectManager/GameObjectManager.h"

LifetimeComp::LifetimeComp(GameObject* _owner) : EngineComponent(_owner), lifetime(9999), currentTime(0)
{
}

LifetimeComp::~LifetimeComp()
{
}

void LifetimeComp::Update()
{
	currentTime += (float)GLHelper::delta_time;

	if (currentTime > lifetime)
	{
		GameObjectManager::GetInstance().ReservationRemoveObject(owner->GetName());
	}
}

bool LifetimeComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		ImGui::InputFloat("lifetime", &lifetime);

		if (DeleteCompButton<LifetimeComp>())
			return false;

		ImGui::TreePop();
	}

	if (DeleteCompMenuItem<LifetimeComp>())
		return false;

	return true;
}

void LifetimeComp::LoadFromJson(const json& _data)
{
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto it = compData->find("lifetime");
		lifetime = it.value();
	}
}

json LifetimeComp::SaveToJson()
{
	json data;

	data["type"] = TypeName;

	json compData;
	compData["lifetime"] = lifetime;

	data["compData"] = compData;
	return data;
}

BaseRTTI* LifetimeComp::CreateLifetimeComponent(GameObject* owner)
{
	return owner->AddComponent<LifetimeComp>();
}