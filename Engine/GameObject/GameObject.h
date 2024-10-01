#pragma once
#include <map>
#include <string>

class BaseComponent;

class GameObject
{
private:
	//GO now will have a container of BaseComponent*
	std::map<std::string, BaseComponent*> component;

public:
	GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;

	//Components in the GO container are ALLOCATED IN THE HEAP, 
	//so. When to GO is destroyed, the GO must be as well
	~GameObject();

	//GO will have a function to add ANY TYPE of component class to their container
	template <typename T>
	T* AddComponent();

	//GO will have a function to find if it already has a component of ANY TYPE
	template <typename T>
	T* GetComponent();

	//GO will have a function to delete ANY TYPE of component
	template <typename T>
	void DeleteComponent();
};

#include "GameObject.inl"