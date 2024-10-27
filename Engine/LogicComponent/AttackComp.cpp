#include "AttackComp.h"

#include "../Prefab/Prefab.h"

#include "../OpenGL/GLHelper.h"
#include "../Editor/Util.h"
#include "../EngineComponent/TransformComp.h"

AttackComp::AttackComp(GameObject* _owner) : LogicComponent(_owner)
{

}

AttackComp::~AttackComp()
{

}

void AttackComp::Update()
{
	TransformComp* t = owner->GetComponent<TransformComp>();
	if (!t) return;

	if (GLHelper::keyState[GLFW_KEY_Z])
	{
		GameObject* obj = Prefab::NewGameObject("Bomb", prefabName);
		obj->GetComponent<TransformComp>()->SetPos(t->GetPos());
		GLHelper::keyState[GLFW_KEY_Z] = GL_FALSE;
	}
}

bool AttackComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		FileSelectComboOnce(prefabName, "Prefab", prefabName, "Assets/Prefab", ".prefab");

		if (DeleteCompButton<AttackComp>())
			return false;

		ImGui::TreePop();
	}

	if (DeleteCompMenuItem<AttackComp>())
		return false;

	return true;
}

void AttackComp::LoadFromJson(const json& _data)
{
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto it = compData->find("prefabName");
		prefabName = it.value();
	}
}

json AttackComp::SaveToJson()
{
	json data;
	data["type"] = TypeName;

	json compData;
	compData["prefabName"] = prefabName;
	data["compData"] = compData;

	return data;
}

BaseRTTI* AttackComp::CreateAttackComponent(GameObject* _owner)
{
	return _owner->AddComponent<AttackComp>();
}
