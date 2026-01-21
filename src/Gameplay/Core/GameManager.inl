#pragma once
#include "GameObject.hpp"

template <class ObjectClass>
std::vector<GameObject*> GameManager::GetAllObjects()
{
    std::vector<GameObject*> vect;
    for (GameObject* object : m_gameObjects)
    {
        if (ObjectClass* casted = dynamic_cast<ObjectClass*>(object))
            vect.push_back(object);
    }
    return vect;
}

template <class ComponentClass>
std::vector<ComponentClass*> GameManager::GetAllComponents()
{
    std::vector<ComponentClass*> vect;
    for (GameObject* object : m_gameObjects)
    {
        if (ComponentClass* casted = dynamic_cast<ComponentClass*>(object))
            vect.push_back(casted);
    }
    return vect;
}

template <class ObjectClass>
ObjectClass* GameManager::CreateObject()
{
    ObjectClass* newObject = new ObjectClass();

    GameObject* casted = dynamic_cast<GameObject*>(newObject);
    if (casted == nullptr)
    {
        delete newObject;
        return nullptr;
    }
    casted->m_pEntity = cpuEngine.CreateEntity();
    m_gameObjects.push_back(casted);
    return newObject;

}