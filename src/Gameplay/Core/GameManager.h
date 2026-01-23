#pragma once
#include "pch.h"
#include "Gameplay/Map.h"

class GameObject;
class Collider;
class Player;
class PlayerController;

class GameManager
{
public:
    GameManager();
    ~GameManager();

    static GameManager& Get();

    void Start();
    void Update();

	void ServerSideStart();
	void ServerSideUpdate();
    
    bool WorldToScreen(XMFLOAT3 const& world, XMFLOAT2& screen, XMFLOAT4X4 const& viewProj, int screenWidth, int screenHeight);
    std::vector<Player*> GetPlayers() const { return m_players; };
    
    void CollisionCheck();

    void AddPlayer(XMFLOAT3 _pos, float _angle, XMINT3 _color, std::string _pseudo, bool _mainPlayer = false);
    void RespawnPlayer(Player* _player, float _minDistanceBetweenPlayers = 30.0f);
    
    template <class ObjectClass> ObjectClass* CreateObject();
    template <class ObjectClass> std::vector<GameObject*> GetAllObjects();
    template <class ComponentClass> std::vector<ComponentClass*> GetAllComponents();
protected:
    
private:
    static GameManager* s_pInstance;

    Map m_map;
    std::vector<cpu_entity*> m_floors;
    cpu_mesh m_pFloorMesh;
    cpu_material m_pFloorMaterial;
    
    PlayerController* m_pPlCrt;
    std::vector<Player*> m_players;
    std::vector<GameObject*> m_gameObjects;
};

#include "GameManager.inl"
