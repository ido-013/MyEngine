#pragma once

#include <map>
#include <string>

#include "GraphicComponent.h"
#include "../Animation/Animation.h"

class AnimatorComp : public GraphicComponent
{
private:
	int ind;
	bool loop;
	double speed;
	double timer;

	std::map<std::string, Animation*> animations;

	Animation* current;
	std::string currentAnimationName;

public:
	AnimatorComp(GameObject* _owner);
	~AnimatorComp();

	void AddAnimation(std::string _name);
	void UpdateAnimation(double _time, std::string _spriteName, std::string _name);
	void SetAnimation(bool _loop, double _speed, std::string _name);

	void Update() override;
	bool Edit() override;

	std::string GetCurrentAnimationName() const { return currentAnimationName; }

	void LoadFromJson(const json& _data) override;
	json SaveToJson() override;

	static BaseRTTI* CreateAnimatorComponent(GameObject* _owner);
	static constexpr const char* TypeName = "AnimatorComp";
};