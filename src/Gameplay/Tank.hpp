#pragma once
#include <iostream>

#include "pch.h"

class Tank
{
public:
    Tank();
    ~Tank() = default;
    void Create(XMFLOAT3 _pos, XMINT3 _color);
    void Destroy();
    void Update();

protected:
    

private:
    cpu_entity* m_pBody;
    cpu_entity* m_pHead;
    cpu_entity* m_pCanon;
    
    cpu_mesh m_meshTank;
    cpu_material m_materialTank;

    float m_speed = 2.5f;
};


inline Tank::Tank() : m_pBody(nullptr), m_pHead(nullptr), m_pCanon(nullptr) { }

inline void Tank::Create(XMFLOAT3 _pos, XMINT3 _color)
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

inline void Tank::Destroy()
{
    m_pBody = cpuEngine.Release(m_pBody);
    m_pHead = cpuEngine.Release(m_pHead);
    m_pCanon = cpuEngine.Release(m_pCanon);
}

inline void Tank::Update()
{
    float dt = cpuTime.delta;
    
    // Move tank
    XMFLOAT3 YPR(0.f, 0.f, 0.f);
    // TODO: Move to PlayerController
    if (cpuInput.IsKey(VK_LEFT))
        YPR.x = -dt;
    if (cpuInput.IsKey(VK_RIGHT))
        YPR.x = dt;
    if (cpuInput.IsKey(VK_UP))
        m_pBody->transform.Move(dt * m_speed);
    if (cpuInput.IsKey(VK_DOWN))
        m_pBody->transform.Move(-dt * m_speed);

    m_pBody->transform.AddYPR(YPR.x, YPR.y, YPR.z);
    
    // Follow tank
    
    m_pCanon->transform.pos.x = m_pBody->transform.pos.x + 2.0f;
    m_pCanon->transform.pos.y = m_pBody->transform.pos.y + 0.5f;
    m_pCanon->transform.pos.z = m_pBody->transform.pos.z;
    m_pCanon->transform.rot = m_pBody->transform.rot;
    
    m_pHead->transform.pos.x = m_pBody->transform.pos.x;
    m_pHead->transform.pos.y = m_pBody->transform.pos.y + 0.5f;
    m_pHead->transform.pos.z = m_pBody->transform.pos.z;
    m_pHead->transform.rot = m_pBody->transform.rot;

    cpuEngine.GetCamera()->transform.pos.x = m_pBody->transform.pos.x;
    cpuEngine.GetCamera()->transform.pos.y = m_pHead->transform.pos.y + 0.5f;
    cpuEngine.GetCamera()->transform.pos.z = m_pBody->transform.pos.z - 5.0f;
    //cpuEngine.GetCamera()->transform.rot = m_pBody->transform.rot;
    
    std::cout << m_pBody->transform.pos.x << " | " << m_pBody->transform.pos.y << " | " << m_pBody->transform.pos.z << '\n';

    /*// Fire
    if ( cpuInput.IsKey(VK_SPACE) )
        cpuApp.SpawnMissile();*/
}