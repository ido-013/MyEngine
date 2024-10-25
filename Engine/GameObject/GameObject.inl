template<typename T>
inline T* GameObject::AddComponent()
{
	T* temp = GetComponent<T>();
	if (temp != nullptr)
	{
		return temp;
	}

	T* comp = new T(this);
	component.insert({ T::TypeName, comp });
	return comp;
}

template <typename T>
inline T* GameObject::AddComponent(BaseComponent* _comp)
{
	T* temp = GetComponent<T>();
	if (temp != nullptr)
	{
		delete _comp;
		return temp;
	}

	component.insert({ T::TypeName, _comp });
	return static_cast<T*>(_comp);
}

template<typename T>
inline T* GameObject::GetComponent()
{
	auto it = component.find(T::TypeName);
	if (it != component.end())
	{
		return static_cast<T*>(it->second);
	}

	return nullptr;
}

template<typename T>
inline void GameObject::DeleteComponent()
{
	auto it = component.find(T::TypeName);
	if (it != component.end())
	{
		delete it->second;
		component.erase(it);
	}
}