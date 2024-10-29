#include "BombComp.h"

#include "../OpenGL/GLHelper.h"
#include "../Editor/Util.h"

#include "../GameObjectManager/GameObjectManager.h"
#include "../Prefab/Prefab.h"

#include "../EngineComponent/TransformComp.h"

BombComp::BombComp(GameObject* _owner) : EngineComponent(_owner), length(1), timer(0), maxTimer(1.5f)
{
}

BombComp::~BombComp()
{

}

void BombComp::Update()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	timer += (float)GLHelper::delta_time;

	if (timer > maxTimer)
	{
		GameObjectManager::GetInstance().ReservationRemoveObject(owner->GetName());

		if (!effectName.empty())
		{
			GameObject* obj = Prefab::NewGameObject("Explosion", effectName);
			obj->GetComponent<TransformComp>()->SetPos(t->GetPos());
		}
	}
}

bool BombComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		ImGui::InputInt("length", &length);

		ImGui::InputFloat("maxTimer", &maxTimer);

		FileSelectComboOnce(effectName, "Effect Name", effectName, "Assets/Prefab", ".prefab");

		if (DeleteCompButton<BombComp>())
			return false;

		ImGui::TreePop();
	}

	if (DeleteCompMenuItem<BombComp>())
		return false;

	return true;
}

void BombComp::LoadFromJson(const json& _data)
{
	// Check how you saved, load from there
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto it = compData->find("length");
		length = it.value(); 
		
		it = compData->find("maxTimer");
		maxTimer = it.value();

		it = compData->find("effectName");
		effectName = it.value();
	}
}

json BombComp::SaveToJson()
{
	json data;

	data["type"] = TypeName;

	json compData;
	compData["length"] = length;
	compData["maxTimer"] = maxTimer;
	compData["effectName"] = effectName;

	data["compData"] = compData;
	return data;
}

BaseRTTI* BombComp::CreateBombComponent(GameObject* _owner)
{
	return _owner->AddComponent<BombComp>();
}

