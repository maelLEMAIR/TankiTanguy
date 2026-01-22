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

    // Ajout d'un constructeur par défaut explicite
    PlayerState()
        : id(0),
          position(0, 0, 0),
          direction(0, 0, 0),
          angle(0.0f),
          life(100.0f),
          inputs()
    {}

    // Constructeur personnalisé déjà utilisé dans l'initialisation
    PlayerState(int _id, XMFLOAT3 _position, XMFLOAT3 _direction, float _angle, Life _life, std::list<int> _inputs)
        : id(_id),
          position(_position),
          direction(_direction),
          angle(_angle),
          life(_life),
          inputs(_inputs)
    {}
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

    MatchState()
    {
        // Initialisation explicite des PlayerState
        players[0] = PlayerState{0, XMFLOAT3(0,0,0), XMFLOAT3(0,0,0), 0.0f, Life(100.0f), std::list<int>()};
        players[1] = PlayerState{1, XMFLOAT3(0,0,0), XMFLOAT3(0,0,0), 0.0f, Life(100.0f), std::list<int>()};
        // La map missiles est déjà initialisée par défaut
    }
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
