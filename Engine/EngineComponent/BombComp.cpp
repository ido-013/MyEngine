#include "BombComp.h"

#include "../OpenGL/GLHelper.h"
#include "../Editor/Util.h"

#include "../GameObjectManager/GameObjectManager.h"
#include "../Prefab/Prefab.h"

#include "../Editor/Editor.h"

#include "../EngineComponent/TransformComp.h"

BombComp::BombComp(GameObject* _owner) : EngineComponent(_owner), length(1), pos(), scale()
{
}

BombComp::~BombComp()
{
	if (Editor::GetInstance().GetMode() == Editor::EDIT)
		return;

	if (!effectName.empty())
	{
		GameObject* obj = Prefab::NewGameObject("Explosion", effectName);
		obj->GetComponent<TransformComp>()->SetPos(pos);

		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				obj = Prefab::NewGameObject("Explosion", effectName);
				obj->GetComponent<TransformComp>()->SetPos({pos.x + (scale.x * dx[j]), pos.y + (scale.y * dy[j])});
			}
		}
	}
}

void BombComp::Update()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	pos = t->GetPos();
	scale = t->GetScale();
}

bool BombComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		ImGui::InputInt("length", &length);

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
	compData["effectName"] = effectName;

	data["compData"] = compData;
	return data;
}

BaseRTTI* BombComp::CreateBombComponent(GameObject* _owner)
{
	return _owner->AddComponent<BombComp>();
}

