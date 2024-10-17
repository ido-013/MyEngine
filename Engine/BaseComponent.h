#pragma once
#include <vector>
#include <string>
#include "GameObject/GameObject.h"
#include "RTTI/BaseRTTI.h"

#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"

class BaseComponent : public BaseRTTI
{
private:
	BaseComponent() = delete;
	BaseComponent(const BaseComponent&) = delete;

protected:
	GameObject* owner;

public:
	BaseComponent(GameObject* _owner) : owner(_owner) {}

	//virtual Destructor!!!
	virtual ~BaseComponent() = default;

	//To support polymorphism we MUST make this interface virtual
	//Update function -> performs the logic, all my inherited classes will use the function with the same syntax
	virtual void Update() = 0;
	virtual bool Edit() = 0;

	GameObject* GetOwner() { return owner; }
};
