#include "PlayerManager.h"
#include <random>
#include "Map.h"
#include "Utils.hpp"
#include "Player.h"

using namespace Utils;

PlayerManager* PlayerManager::s_pInstance = nullptr;

PlayerManager::PlayerManager()
{
    s_pInstance = this;
}

PlayerManager::~PlayerManager()
{
    delete m_pPlayers;
    delete m_pMap;
}

void PlayerManager::Init(std::vector<Player*>& _players, Map const& _map)
{
    m_pPlayers = &_players;
    m_pMap = &_map;
}

PlayerManager* PlayerManager::Get()
{
    return s_pInstance;
}

void PlayerManager::Update()
{
    for (Player* player : *m_pPlayers)
        player->Update();
}

void PlayerManager::RespawnPlayer(Player* _player, float _minDistanceBetweenPlayers)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, m_pMap->m_map.size() - 1);
    constexpr int MAX_ATTEMPTS = 100;
    int attempts = 0;

    while (attempts < MAX_ATTEMPTS)
    {
        attempts++;
        
        XMINT2 tile = m_pMap->m_map[dist(gen)];
        XMFLOAT2 spawnPos = { (float)tile.x, (float)tile.y };

        bool valid = true;
        for (Player* other : *m_pPlayers)
        {
            if (other == _player)
                continue;
            
            if (other->IsDead())
                continue;

            XMFLOAT2 otherPos = XMFLOAT2(other->GetPos().x, other->GetPos().z);
            float d = Distance(spawnPos, otherPos);
            if (d < _minDistanceBetweenPlayers)
            {
                valid = false;
                break;
            }
        }

        if (valid)
        {
            _player->Revive(XMFLOAT3(spawnPos.x, 0.0f,  spawnPos.y));
            return;
        }
    }

    // Fallback si aucune position trouvée
    _player->Revive({ 0.f, 0.f, 0.f });
}


