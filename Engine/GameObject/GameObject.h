#pragma once
#include <map>
#include <string>

class BaseComponent;

class GameObject
{
private:
	//GO now will have a container of BaseComponent*
	std::map<std::string, BaseComponent*> component;
	std::string name;

	GameObject(std::string _name);
	//Components in the GO container are ALLOCATED IN THE HEAP, 
	//so. When to GO is destroyed, the GO must be as well
	~GameObject();

public:
	GameObject(const GameObject& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;

	//GO will have a function to add ANY TYPE of component class to their container
	template <typename T>
	T* AddComponent();

	template <typename T>
	T* AddComponent(BaseComponent* comp);

	//GO will have a function to find if it already has a component of ANY TYPE
	template <typename T>
	T* GetComponent();

	//GO will have a function to delete ANY TYPE of component
	template <typename T>
	void DeleteComponent();

	const std::map<std::string, BaseComponent*>& GetAllComponent() { return component; }
	
	const std::string& GetName() { return name; }
	void SetName(const std::string& _name) { name = _name; }

	friend class GameObjectManager;
};

#include "GameObject.inl"