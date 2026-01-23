#pragma once 
#include "pch.h"

#include "Missile.h"

class Player;
class GameManager;

class Weapon
{
public:
    Weapon(cpu_transform& _wpTransform, Player& _owner) : m_pWeaponTransform(&_wpTransform), m_pOwner(&_owner) {};
    ~Weapon();
    void Update();
    void Shoot();
protected:
    
private:
    cpu_transform* m_pWeaponTransform;
    std::vector<Missile*> m_pMissiles;
    Player* m_pOwner;
    float m_fireDuration = 1.5f;
    float m_fireDurationMax = 1.5f;
};

