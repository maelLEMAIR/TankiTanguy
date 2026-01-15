#pragma once
#include "pch.h"

class Missile
{
public:
    Missile() = default;
    ~Missile();
    void Create(cpu_transform& _transform);
    void Update();

    cpu_entity& GetEntity() const {return *m_pMissile; };
    
protected:
    
private:
    XMFLOAT3 m_firstPos;
    
    cpu_entity* m_pMissile;
    cpu_mesh m_meshMissile;
    cpu_material m_missileMaterial;

    float m_speed = 2.5f;
};

inline Missile::~Missile()
{
    m_pMissile = cpuEngine.Release(m_pMissile);
}

inline void Missile::Create(cpu_transform& _transform)
{
    m_firstPos = _transform.pos;
    m_missileMaterial.color = cpu::ToColor(87, 87, 87);
    m_meshMissile.CreateSphere();
    
    m_pMissile = cpuEngine.CreateEntity();
    m_pMissile->pMesh = &m_meshMissile;
    m_pMissile->pMaterial = &m_missileMaterial;
    m_pMissile->transform.pos = _transform.pos;
    m_pMissile->transform.pos.z += 20.0f;
    m_pMissile->transform.SetRotation(_transform);
    m_pMissile->transform.sca = XMFLOAT3(0.25f, 0.25f, 0.25f);
}

inline void Missile::Update()
{
    float dt = cpuTime.delta;
    m_pMissile->transform.Move(m_speed * dt);
}