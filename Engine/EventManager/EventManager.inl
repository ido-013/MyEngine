template<typename T>
inline void EventManager::AddEvent(Entity* _src, Entity* _dst)
{
	if (_dst != nullptr)
	{
		Event* event = new T(_src, _dst);
		events.push(event);
	}
	else
	{
		for (auto& it : entities)
		{
			Event* event = new T(_src, it);
			events.push(event);
		}
	}
}
