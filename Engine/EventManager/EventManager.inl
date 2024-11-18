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
