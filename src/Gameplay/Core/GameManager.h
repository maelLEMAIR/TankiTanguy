#pragma once
#include "pch.h"
#include "PlayerController.hpp"

class GameObject;
class Collider;
class Player;

class GameManager
{
public:
    GameManager();
    ~GameManager();
    static GameManager* Get();
    void Start();
    void Update();
    void CollisionCheck();
    template <class ObjectClass> ObjectClass* CreateObject();
    template <class ObjectClass> std::vector<GameObject*> GetAllObjects();
    template <class ComponentClass> std::vector<ComponentClass*> GetAllComponents();
protected:
    
private:
    static GameManager* s_pInstance;
    
    PlayerController* m_pPlCrt;
    std::vector<GameObject*> m_gameObjects;
};

#include "GameManager.inl"
