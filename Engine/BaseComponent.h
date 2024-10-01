#pragma once
#include <vector>
#include <string>
#include "../GameObject/GameObject.h"

class BaseComponent
{
private:
	BaseComponent() = delete;
	BaseComponent(const BaseComponent& ohter) = delete;

protected:
	GameObject* owner;

public:
	BaseComponent(GameObject* _owner) : owner(_owner) {}

	//virtual Destructor!!!
	virtual ~BaseComponent() = default;

	//To support polymorphism we MUST make this interface virtual
	//Update function -> performs the logic, all my inherited classes will use the function with the same syntax
	virtual void Update() = 0;

	GameObject* GetOwner() { return owner; }
};
