#include "EventManager.h"
#include <iostream>

EventManager::EventManager() {}

EventManager::~EventManager()
{
    DeleteUndispahchEvent();
}

void EventManager::AddEvent(Event* _event)
{
    events.push(_event);
}

void EventManager::AddEntity(Entity* _entity)
{
    entities.push_back(_entity);
}

void EventManager::DeleteEntity(Entity* _entity)
{
    for (auto it = entities.begin(); it != entities.end(); it++)
    {
        if (*it == _entity)
        {
            entities.erase(it);
            return;
        }
    }
}

void EventManager::DispatchAllEvents()
{
    while (!events.empty())
    {
        events.front()->DispatchEvent();

        if (events.empty())
            break;

        delete events.front();
        events.pop();
    }
}

void EventManager::DeleteUndispahchEvent()
{
    while (!events.empty())
    {
        delete events.front();
        events.pop();
    }
}
