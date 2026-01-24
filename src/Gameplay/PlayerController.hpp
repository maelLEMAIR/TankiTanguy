#pragma once
#include "Player.h"

class PlayerController
{
public:
    PlayerController() = default;
    ~PlayerController() = default;
    
    void Init(Player& _player) { m_pPlayer = &_player; }
    void HandleInput();
    
private:
    Player* m_pPlayer = nullptr;
};

inline void PlayerController::HandleInput()
{
    if (!m_pPlayer || m_pPlayer->IsDead())
        return;
    
    if (cpuInput.IsKey(VK_UP))
        m_pPlayer->Move(1);
    if (cpuInput.IsKey(VK_DOWN))
        m_pPlayer->Move(-1);
    if (cpuInput.IsKey(VK_RIGHT))
        m_pPlayer->Turn(1);
    if (cpuInput.IsKey(VK_LEFT))
        m_pPlayer->Turn(-1);
    
    if (cpuInput.IsKeyDown('N'))
        m_pPlayer->GetLife().TakeDamage(5);
    
    if (cpuInput.IsKey(VK_SPACE))
        m_pPlayer->Shoot();
}