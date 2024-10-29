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
        if (*it == _comp)
        {
            component.erase(it);
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
    DEBUG_PROFILER_START(typeid(T).name());
    for (auto it = component.begin(); it != component.end(); it++)
    {
        (*it)->Update();
    }
    DEBUG_PROFILER_END;
}