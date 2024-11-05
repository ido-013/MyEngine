#pragma once

#include <queue>
#include <vector>

#include "../Event/Event.h"
#include "../Event/Entity.h"

class EventManager
{
private:
	std::queue<Event*> events;
	std::vector<Entity*> entities;

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
	void AddEvent(Entity* _src, Entity* _dst);
	
	void AddEvent(Event* _event);

	void AddEntity(Entity* _entity);
	void DeleteEntity(Entity* _entity);

	// DispatchAllEvents
	void DispatchAllEvents();

	// delete undispatched events if any on destructor
	void DeleteUndispahchEvent();
};

#include "EventManager.inl"
