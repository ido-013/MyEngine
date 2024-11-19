#include "EventManager.h"
template<typename T>
inline void EventManager::AddEvent(GameObject* _src, GameObject* _dst)
{
	if (_dst != nullptr)
	{
		Event* event = new T(_src, _dst);
		events.push(event);
	}
	else
	{
		for (auto& it : GameObjectManager::GetInstance().GetAllObject())
		{
			Event* event = new T(_src, it);
			events.push(event);
		}
	}
}

template<typename T>
inline void EventManager::AddEvent(T* _event, const std::string& _layerName)
{
	int layer = LayerManager::GetInstance().GetLayerInd(_layerName);

	for (auto& it : GameObjectManager::GetInstance().GetAllObject())
	{
		if (layer == it.second->GetLayer())
		{
			_event->dst = it.second;
			AddEvent(new T(*_event));
		}
	}

	delete _event;
}