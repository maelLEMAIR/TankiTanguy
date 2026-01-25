#include "Weapon.h"
#include "Core/GameManager.h"

Weapon::~Weapon() { m_pMissiles.clear(); } 


void Weapon::Shoot()
{
    if (m_fireDuration >= m_fireDurationMax)
    {
        GameManager& s_pI = GameManager::Get();
        Missile* missile = s_pI.CreateObject<Missile>(m_isServer);
        missile->Init(*m_pWeaponTransform, *m_pOwner);
        m_pMissiles.push_back(missile);
        m_fireDuration = 0.0f;
    }
}

void Weapon::Update()
{
    float dt = cpuTime.delta;
    m_fireDuration += dt;

    // Update Missile
    for (auto it = m_pMissiles.begin(); it != m_pMissiles.end(); )
    {
        Missile* pMissile = *it;
        if (pMissile->IsNeedToBeDestroyed())
            it = m_pMissiles.erase(it);
        else
        {
            pMissile->Update();
            ++it;
        }
    }
}