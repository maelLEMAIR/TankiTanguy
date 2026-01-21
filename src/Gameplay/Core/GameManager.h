#pragma once
#include "pch.h"
#include "Player.hpp"
#include "Missile.hpp"
#include "Life.hpp"

class GameObject;
class Collider;

struct PlayerState
{
    int id;
    XMFLOAT3 position;
	XMFLOAT3 direction;
    float angle;
    Life life;
	std::list<int> inputs;
};

struct MissileState
{
    int id;
	int ownerId;
	XMFLOAT3 position;
	XMFLOAT3 direction;
	int ttl;  // time to live in frames
};

struct MatchState
{
    PlayerState players[2];
    std::map<int, MissileState*> missiles;
};

class GameManager
{
public:
    GameManager();
    ~GameManager();

    static GameManager* Get();

    void Start();
    void Update();
	void Stop();

    void CollisionCheck();

	MatchState& GetMatchState() { return m_matchState; }

    template <class ObjectClass> ObjectClass* CreateObject();
    template <class ObjectClass> std::vector<GameObject*> GetAllObjects();
    template <class ComponentClass> std::vector<ComponentClass*> GetAllComponents();
protected:
    
private:
    static GameManager* s_pInstance;
    
    std::vector<GameObject*> m_gameObjects;
	MatchState m_matchState;
};

#include "GameManager.inl"
