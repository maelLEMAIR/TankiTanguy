#pragma once
#include "Map.h"
#include "pch.h"

class Player;

class PlayerManager
{
public:
    PlayerManager();
    ~PlayerManager();
    void Init(std::vector<Player*>& _players, Map const& _map);
    static PlayerManager* Get();
    void Update();
    void RespawnPlayer(Player* _player, float _minDistanceBetweenPlayers = 30.f);

protected:
private:
    static PlayerManager* s_pInstance;
    
    std::vector<Player*>* m_pPlayers;
    Map const* m_pMap;
};
