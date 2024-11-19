#pragma once

#include <queue>
#include <list>

#include "../GameObjectManager/GameObjectManager.h"

#include "../Event/Event.h"
#include "../GameObject/GameObject.h"

class EventManager
{
private:
	std::queue<Event*> events;

	EventManager();

	EventManager(const EventManager&) = delete;
	const EventManager& operator=(const EventManager) = delete;

	~EventManager();

public:
	static EventManager& GetInstance()
	{
		static EventManager instance;
		return instance;
	}

	// Add an event (event*) this are pointers to dynamic memory, called as the following: AddEvent(new Event);
	template <typename T>
	void AddEvent(GameObject* _src, GameObject* _dst = nullptr);
	template <typename T>
	void AddEvent(T* _event, const std::string& _layerName);
	
	void AddEvent(Event* _event);

	// DispatchAllEvents
	void DispatchAllEvents();

	// delete undispatched events if any on destructor
	void DeleteUndispahchEvent();
};

#include "EventManager.inl"
