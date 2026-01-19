#pragma once 
#include "pch.h"

#include "Missile.hpp"

class Weapon
{
public:
    Weapon(cpu_transform& _wpTransform) : m_pWeaponTransform(&_wpTransform) {};
    ~Weapon();
    void Init();
    void Update();
    void Shoot();
protected:
    
private:
    cpu_transform* m_pWeaponTransform;
    std::vector<Missile*> m_pMissiles;

    float m_fireDuration = 1.5f;
    float m_fireDurationMax = 1.5f;
};

inline Weapon::~Weapon()
{
    m_pMissiles.clear();
}


inline void Weapon::Shoot()
{
    if (m_fireDuration >= m_fireDurationMax)
    {
        Missile* missile = new Missile();
        missile->Create(*m_pWeaponTransform);
        m_pMissiles.push_back(missile);
        m_fireDuration = 0.0f;
    }
}

inline void Weapon::Update()
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

