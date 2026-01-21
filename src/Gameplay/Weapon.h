#pragma once 
#include "pch.h"

#include "Missile.hpp"

class GameManager;

class Weapon
{
public:
    Weapon(cpu_transform& _wpTransform) : m_pWeaponTransform(&_wpTransform) {};
    ~Weapon();
    void Update();
    void Shoot();
protected:
    
private:
    cpu_transform* m_pWeaponTransform;
    std::vector<Missile*> m_pMissiles;

    float m_fireDuration = 1.5f;
    float m_fireDurationMax = 1.5f;
};

