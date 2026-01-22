#pragma once
#include "pch.h"
#include "Missile.hpp"
#include "Life.hpp"

class GameObject;
class Collider;
class Player;
class PlayerController;

struct PlayerState
{
	Player* player;
    PlayerController* playerCtrl;
    std::vector<int> inputs;
};

struct MatchState
{
    PlayerState players[2];
    std::vector<Missile*> missiles;
};

class GameManager
{
public:
    GameManager();
    ~GameManager();

    static GameManager* Get();

    void ClientSideStart();
    void ClientSideUpdate();
	void ClientSideStop();

	void ServerSideStart();
    void ServerSideUpdate();
	void ServerSideStop();

    void CollisionCheck(); 
	void UpdateMatchState();

	MatchState& GetMatchState() { return m_matchState; }
	bool IsServer() const { return true; }

    template <class ObjectClass> ObjectClass* CreateObject();
    template <class ObjectClass> std::vector<GameObject*> GetAllObjects();
    template <class ComponentClass> std::vector<ComponentClass*> GetAllComponents();
protected:
    
private:
    static GameManager* s_pInstance;
    
    std::vector<GameObject*> m_gameObjects;
	PlayerController* m_pLocalPlayerCtrl;
	MatchState m_matchState;
};

#include "GameManager.inl"
