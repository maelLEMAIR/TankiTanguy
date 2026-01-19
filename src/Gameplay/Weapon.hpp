#pragma once 
#include "pch.h"

#include "Missile.hpp"

class Weapon
{
public:
    Weapon(float& _angle, cpu_transform& _wpTransform) : m_pWeaponTransform(&_wpTransform) {};
    ~Weapon();
    void Init();
    void Update();
    void Shoot();
protected:
    
private:
    cpu_transform* m_pWeaponTransform;
    std::vector<Missile*> m_pMissiles;
};

inline Weapon::~Weapon()
{
    m_pMissiles.clear();
}


inline void Weapon::Shoot()
{
    Missile* missile = new Missile();
    missile->Create(*m_pWeaponTransform);
    m_pMissiles.push_back(missile);
}

inline void Weapon::Update()
{
    float dt = cpuTime.delta;
    
    // Move missiles
    for ( auto it=m_pMissiles.begin() ; it!=m_pMissiles.end() ; ++it )
    {
        Missile* pMissile = *it;
        pMissile->Update();
    }
}

