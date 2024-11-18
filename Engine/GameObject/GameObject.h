#pragma once

#include <map>
#include <string>

#include "../LayerManager/LayerManager.h"

class BaseComponent;

class GameObject
{
private:
	GameObject(std::string _name);
	//Components in the GO container are ALLOCATED IN THE HEAP, 
	//so. When to GO is destroyed, the GO must be as well
	~GameObject();

	//GO now will have a container of BaseComponent*
	std::map<std::string, BaseComponent*> component;

	int layer;

	std::string name;
	std::string prefabName;
	std::string layerName;

public:
	bool selected;

	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

	//GO will have a function to add ANY TYPE of component class to their container
	template <typename T>
	T* AddComponent();

	//GO will have a function to find if it already has a component of ANY TYPE
	template <typename T>
	T* GetComponent();

	//GO will have a function to delete ANY TYPE of component
	template <typename T>
	void DeleteComponent();

	bool IsHaveComponent(const std::string& _typeName);

	const std::map<std::string, BaseComponent*>& GetAllComponent() { return component; }
	
	const std::string& GetName() { return name; }
	const std::string& GetPrefabName() { return prefabName; }
	const int& GetLayer() const { return layer; }
	const std::string& GetLayerName() { return layerName; }

	void SetLayer(const int& _layer);

	friend class GameObjectManager;
};

#include "GameObject.inl"


