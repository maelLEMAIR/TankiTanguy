#pragma once
#include <iostream>

#include "Missile.hpp"
#include "pch.h"

class Player
{
public:
    Player();
    ~Player() = default;
    void Create(XMFLOAT3 _pos, XMINT3 _color);
    void Destroy();
    void Update();
    void Move(float _dir);
    void Turn(float _dir);
    void Shoot();

protected:
    

private:
    cpu_entity* m_pBody;
    cpu_entity* m_pHead;
    cpu_entity* m_pCanon;
    
    cpu_mesh m_meshTank;
    cpu_material m_materialTank;

    std::vector<Missile*> m_pMissiles;
    float m_speed = 2.5f;
};


inline Player::Player() : m_pBody(nullptr), m_pHead(nullptr), m_pCanon(nullptr) { }

inline void Player::Create(XMFLOAT3 _pos, XMINT3 _color)
{
    m_materialTank.color = cpu::ToColor(_color.x, _color.y, _color.z);
    m_meshTank.CreateCube();
    
    m_pBody = cpuEngine.CreateEntity();
    m_pBody->pMesh = &m_meshTank;
    m_pBody->pMaterial = &m_materialTank;
    m_pBody->transform.pos = _pos;
    m_pBody->transform.sca = XMFLOAT3(1.0f, 1.0f, 2.0f);

    m_pHead = cpuEngine.CreateEntity();
    m_pHead->pMesh = &m_meshTank;
    m_pHead->pMaterial = &m_materialTank;
    m_pHead->transform.pos = _pos;
    m_pHead->transform.sca = XMFLOAT3(0.75f, 0.5f, 0.75f);

    m_pCanon = cpuEngine.CreateEntity();
    m_pCanon->pMesh = &m_meshTank;
    m_pCanon->pMaterial = &m_materialTank;
    m_pCanon->transform.pos = _pos;
    m_pCanon->transform.sca = XMFLOAT3(0.25f, 0.25f, 3.0f);
}

inline void Player::Destroy()
{
    m_pBody = cpuEngine.Release(m_pBody);
    m_pHead = cpuEngine.Release(m_pHead);
    m_pCanon = cpuEngine.Release(m_pCanon);
    m_pMissiles.clear();
}

inline void Player::Update()
{
    float dt = cpuTime.delta;
    // Move missiles
    for ( auto it=m_pMissiles.begin() ; it!=m_pMissiles.end() ; ++it )
    {
        Missile* pMissile = *it;
        pMissile->Update();
    }
        
    // Follow tank
    // m_pCanon->transform.pos.x = m_pBody->transform.pos.x;
    // m_pCanon->transform.pos.y = m_pBody->transform.pos.y + 0.5f;
    // m_pCanon->transform.pos.z = m_pBody->transform.pos.z + 1.f;
    //
    // m_pHead->transform.pos.x = m_pBody->transform.pos.x;
    // m_pHead->transform.pos.y = m_pBody->transform.pos.y;
    // m_pHead->transform.pos.z = m_pBody->transform.pos.z;

    //cpuEngine.GetCamera()->transform.rot = m_pBody->transform.rot;
    if (cpuInput.IsKeyDown(VK_LBUTTON))
    {
        cpuEngine.GetCamera()->transform.pos.x = m_pHead->transform.pos.x;
        cpuEngine.GetCamera()->transform.pos.y = m_pHead->transform.pos.x + 5.f;
        cpuEngine.GetCamera()->transform.pos.z = m_pHead->transform.pos.x;
        cpuEngine.GetCamera()->transform.SetYPR(0.0f, 90.f, 0.0f);
    }
    if (cpuInput.IsKeyDown(VK_RBUTTON))
    {
        cpuEngine.GetCamera()->transform.pos.x = m_pHead->transform.pos.x;
        cpuEngine.GetCamera()->transform.pos.y = m_pHead->transform.pos.y + 0.5f;
        cpuEngine.GetCamera()->transform.pos.z = m_pHead->transform.pos.z - 5.0f;
        cpuEngine.GetCamera()->transform.SetYPR(0.0f, 0.0f, 0.0f);
    }
}

inline void Player::Move(float _dir)
{
    float dt = cpuTime.delta;
    m_pBody->transform.Move(dt * m_speed * _dir);
}

inline void Player::Turn(float _dir)
{
    float dt = cpuTime.delta;
    m_pBody->transform.AddYPR(dt * _dir);
    m_pHead->transform.OrbitAroundAxis(m_pBody->transform.pos, CPU_UP, 0.f, dt * _dir);
    m_pCanon->transform.OrbitAroundAxis(m_pBody->transform.pos, CPU_UP, 1.f, dt * _dir);
}

inline void Player::Shoot()
{
    Missile* missile = new Missile();
    missile->Create(m_pCanon->transform);
    m_pMissiles.push_back(missile);
}
