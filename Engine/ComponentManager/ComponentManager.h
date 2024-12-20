#pragma once
#include <vector>
#include <list>

#include "../Profiler/Profiler.h"

template <typename T>
class ComponentManager
{
private:
	ComponentManager();
	ComponentManager(const ComponentManager&) = delete;
	const ComponentManager& operator=(const ComponentManager&) = delete;
	~ComponentManager();

	std::list<T*> component;

public:
	static ComponentManager& GetInstance()
	{
		static ComponentManager instance;
		return instance;
	};

	void AddComp(T* _comp);
	void DelComp(T* _comp);

	void Update();
};

#include "ComponentManager.inl"
