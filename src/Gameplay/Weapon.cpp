#include "Weapon.h"
#include "Core/GameManager.h"

Weapon::~Weapon() { m_pMissiles.clear(); } 


void Weapon::Shoot()
{
    if (m_fireDuration >= m_fireDurationMax)
    {
        Missile* missile = GameManager::Get()->CreateObject<Missile>();
        missile->Init(*m_pWeaponTransform);
        m_pMissiles.push_back(missile);
        m_fireDuration = 0.0f;
    }
}

void Weapon::Update()
{
    float dt = cpuTime.delta;
    m_fireDuration += dt;

    // Move missiles
    for ( auto it=m_pMissiles.begin() ; it!=m_pMissiles.end() ; ++it )
    {
        Missile* pMissile = *it;
        pMissile->Update();
    }
}