#pragma once
#include "pch.h"
#include "PlayerController.hpp"

class Collider;
class Player;
class PlayerManager;

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


    PlayerManager* m_pPlayerManager = nullptr;
    PlayerController* m_pPlCrt;
    std::vector<GameObject*> m_gameObjects;
};

#include "GameManager.inl"
