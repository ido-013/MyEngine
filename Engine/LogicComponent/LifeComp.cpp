#include "LifeComp.h"

#include "../OpenGL/GLHelper.h"
#include "../Editor/Util.h"

#include "../GameObjectManager/GameObjectManager.h"
#include "../Prefab/Prefab.h"

#include "../GraphicComponent/SpriteComp.h"

LifeComp::LifeComp(GameObject* _owner) : LogicComponent(_owner), life(3), maxLife(3), isImmune(false), immuneTime(1.5f), timer(0),
										 onEffect(false), effectName()
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
			owner->GetComponent<SpriteComp>()->SetColor(1.f, 1.f, 1.f);
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

		FileSelectComboOnce(effectName, "Effect Name", effectName, "Assets/Prefab", ".prefab");

		ImGui::Checkbox("onEffect", &onEffect);

		ImGui::Separator();

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
		owner->GetComponent<SpriteComp>()->SetColor(0.5f, 0.f, 0.f);
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

		if (onEffect)
		{
			Prefab::NewGameObject("deathEffect", effectName);
		}
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

		it = compData->find("onEffect");
		onEffect = it.value();

		it = compData->find("effectName");
		effectName = it.value();
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
	compData["onEffect"] = onEffect;
	compData["effectName"] = effectName;

	data["compData"] = compData;

	return data;
}

BaseRTTI* LifeComp::CreateLifeComponent(GameObject* _owner)
{
	return _owner->AddComponent<LifeComp>();
}

