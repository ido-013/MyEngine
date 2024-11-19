#include "EventManager.h"

#include <iostream>

#include "../Event/EventHandler.h"

EventManager::EventManager() {}

EventManager::~EventManager()
{
    DeleteUndispahchEvent();
}

void EventManager::AddEvent(Event* _event)
{
    if (_event->src == nullptr || _event->dst == nullptr)
        return;

    events.push(_event);
}

void EventManager::DispatchAllEvents()
{
    while (!events.empty())
    {
        EventHandler::GetInstance().OnEvent(events.front());

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
