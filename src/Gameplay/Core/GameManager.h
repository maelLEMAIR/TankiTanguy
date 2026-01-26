#pragma once
#include "pch.h"

class GameObject;
class Collider;
class Player;
class PlayerController;
struct Map;

class GameManager
{
public:
    GameManager(bool _isServer);
    ~GameManager();

    static GameManager& Get();
    
    void Start();
    void Update();

	void ServerSideStart();
	void ServerSideUpdate();
    
    bool WorldToScreen(XMFLOAT3 const& world, XMFLOAT2& screen, XMFLOAT4X4 const& viewProj, int screenWidth, int screenHeight);
    
    void CollisionCheck();
    void AddPlayer(XMFLOAT3 _pos, float _angle, XMINT3 _color, std::string _pseudo, bool _isServer, bool _mainPlayer = false);
    void RespawnPlayer(Player* _player, float _minDistanceBetweenPlayers = 30.0f);

    Player* GetMainPlayer() const { return m_mainPlayer; };
    std::vector<Player*> GetPlayers() { return m_players; };

    template <class ObjectClass> ObjectClass* CreateObject(bool _isServer);
    template <class ObjectClass> std::vector<GameObject*> GetAllObjects();
    template <class ComponentClass> std::vector<ComponentClass*> GetAllComponents();
protected:
    
private:
    static GameManager* s_pInstance;
    
    bool m_isServer = false;
    
    std::vector<GameObject*> m_gameObjects;
    Map* m_pMap = nullptr;
    
    PlayerController* m_pPlCrt = nullptr;
    Player* m_mainPlayer = nullptr;
    std::vector<Player*> m_players;
};

#include "GameManager.inl"
