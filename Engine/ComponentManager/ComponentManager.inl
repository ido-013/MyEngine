template <typename T>
inline void ComponentManager<T>::AddComp(T* _comp)
{
    component.push_front(_comp);
}

template<typename T>
inline void ComponentManager<T>::DelComp(T* _comp)
{
    delComp = _comp;
}

template <typename T>
ComponentManager<T>::ComponentManager() {}

template <typename T>
ComponentManager<T>::~ComponentManager() {}

template <typename T>
void ComponentManager<T>::Update()
{
    for (auto it = component.begin(); it != component.end(); )
    {
        if (*it == delComp)
        {
            it = component.erase(it);
            delComp = nullptr;
        }
        else
        {
            (*it)->Update();
            it++;
        }
    }
}