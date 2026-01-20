#pragma once
#include "pch.h"

class Life
{
public:
    Life(float _nbrLife);
    ~Life() = default;
    bool IsDead() { return m_currentLife <= 0; }
    void Heal(float _heal) { m_currentLife += _heal; }
    void TakeDamage(float _damage) {m_currentLife -= _damage;};
    
protected:
    
private:
    float m_maxLife;
    float m_currentLife;
};

inline Life::Life(float _nbrLife)
{
    m_maxLife = _nbrLife;
    m_currentLife = m_maxLife;
}
