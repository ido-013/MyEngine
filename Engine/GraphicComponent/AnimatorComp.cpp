#include "AnimatorComp.h"

#include "SpriteComp.h"

#include "../OpenGL/GLHelper.h"

AnimatorComp::AnimatorComp(GameObject* _owner) : GraphicComponent(_owner), loop(false), speed(1.f),
												 current(nullptr), timer(0), ind(0)
{

}

AnimatorComp::~AnimatorComp()
{
	for (auto& it : animations)
	{
		delete it.second;
	}
}

void AnimatorComp::AddAnimation(std::string _name)
{
	animations.insert({ _name, new Animation });
}

void AnimatorComp::UpdateAnimation(double _time, std::string _spriteName, std::string _name)
{
	auto it = animations.find(_name);

	if (it != animations.end())
	{
		it->second->AddFrame(_time, _spriteName);
	}
}

void AnimatorComp::SetAnimation(bool _loop, double _speed, std::string _name)
{
	if (currentAnimationName.compare(_name) != 0 && (loop || (!loop && current == nullptr)))
	{
		loop = _loop;
		speed = _speed;

		ind = 0;
		timer = 0;

		auto it = animations.find(_name);
		if (it != animations.end())
		{
			current = it->second;
			currentAnimationName = _name;  // 현재 애니메이션 이름을 저장
		}
	}
}

void AnimatorComp::Update()
{
	SpriteComp* s = owner->GetComponent<SpriteComp>();

	if (!s || !current)  // SpriteComp 또는 현재 애니메이션이 설정되지 않은 경우 반환
		return;

	if (current->frame.empty())
		return;

	if (timer == 0)
	{
		s->SetTexture(current->frame[ind].second);
	}

	timer += float(GLHelper::delta_time) * speed;

	if (timer > current->frame[ind].first)
	{
		timer = 0;
		ind++;

		if (ind >= current->size)
		{
			ind = 0;

			if (!loop)  // 루프하지 않을 경우 애니메이션 중지
			{
				current = nullptr;
				return;
			}
		}

		// 애니메이션 프레임 갱신
		s->SetTexture(current->frame[ind].second);
	}
}

bool AnimatorComp::Edit()
{
	if (ImGui::TreeNode(TypeName))
	{
		if (DeleteCompButton<SpriteComp>())
			return false;

		ImGui::TreePop();
	}

	if (DeleteCompMenuItem<SpriteComp>())
		return false;

	return true;
}

void AnimatorComp::LoadFromJson(const json& _data)
{
	auto compData = _data.find("compData");

	if (compData != _data.end())
	{
		auto it = compData->find("animation");

		if (it == compData->end())
			return;

		for (auto& aniIt : *it)
		{
			std::string name = aniIt.find("name").value();
			AddAnimation(name);

			for (auto& frameIt : *aniIt.find("frame"))
			{
				UpdateAnimation(frameIt[0], frameIt[1], name);
			}
		}
	}
}

json AnimatorComp::SaveToJson()
{
	json data;
	data["type"] = TypeName;

	json compData;

	json aniData;
	for (auto& it : animations)
	{
		aniData["name"] = it.first;

		for (auto& aniIt : it.second->frame)
		{
			aniData["frame"].push_back({ aniIt.first, aniIt.second });
		}

		compData["animation"].push_back(aniData);
	}

	data["compData"] = compData;

	return data;
}

BaseRTTI* AnimatorComp::CreateAnimatorComponent(GameObject* _owner)
{
	return _owner->AddComponent<AnimatorComp>();
}
