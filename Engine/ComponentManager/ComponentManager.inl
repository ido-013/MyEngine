template <typename T>
inline void ComponentManager<T>::AddComp(T* _comp)
{
    component.push_back(_comp);
}

template<typename T>
inline void ComponentManager<T>::DelComp(T* _comp)
{
    for (auto it = component.begin(); it != component.end(); it++)
    {
        if ((*it) == _comp)
        {
            *it = nullptr;
            return;
        }
    }
}

template <typename T>
ComponentManager<T>::ComponentManager() {}

template <typename T>
ComponentManager<T>::~ComponentManager() {}

template <typename T>
void ComponentManager<T>::Update()
{
    for (auto it : component)
    {
        if (it != nullptr)
            it->Update();
    }

    for (auto it = component.begin(); it != component.end(); )
    {
        if (*it == nullptr)
        {
            it = component.erase(it);
        }
        else
        {
            it++;
        }
    }
}