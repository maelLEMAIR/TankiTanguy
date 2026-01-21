#pragma once
#include "pch.h"
#include "Core/GameObject.hpp"
#include "Core/SphereCollider.h"

class Missile : public GameObject, public SphereCollider
{
public:
    Missile();
    ~Missile();
    void Init(cpu_transform& _transform);
    void Update();
    void OnCollisionEnter(Collider* pOther) override;

    cpu_entity& GetEntity() const {return *m_pEntity; };
    
protected:
    
private:
    XMFLOAT3 m_firstPos;
    cpu_mesh m_meshMissile;
    cpu_material m_missileMaterial;

    float m_speed = 10.0f;
};

inline Missile::Missile() : SphereCollider(XMFLOAT3(1.0f, 1.0f, 1.0f), 1.0f)
{
    m_firstPos = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

inline Missile::~Missile()
{
    GameObject::~GameObject();
}

inline void Missile::Init(cpu_transform& _transform)
{
    m_firstPos = _transform.pos;
    m_missileMaterial.color = cpu::ToColor(87, 87, 87);
    m_meshMissile.CreateSphere();

    XMFLOAT3 forward = _transform.dir;
    
    m_pEntity->pMesh = &m_meshMissile;
    m_pEntity->pMaterial = &m_missileMaterial;
    m_pEntity->transform.pos = _transform.pos;
    m_pEntity->transform.pos.x += forward.x * 2.0f;
    m_pEntity->transform.pos.z += forward.z * 2.0f;
    
    m_pOwner = this;
    m_pEntity->transform.SetRotation(_transform);
    m_pEntity->transform.sca = XMFLOAT3(0.4f, 0.4f, 0.4f);
    m_center = m_pEntity->transform.pos;
    m_radius = 0.4f;
}

inline void Missile::Update()
{
    float dt = cpuTime.delta;
    XMFLOAT3 pos = m_pEntity->transform.pos;
    float dist = sqrt(pow(pos.x - m_firstPos.x, 2) + pow(pos.z - m_firstPos.z, 2));
    if (dist > 25.f) m_pEntity->dead = true;
    m_pEntity->transform.Move(m_speed * dt);
    m_center = m_pEntity->transform.pos;
}

inline void Missile::OnCollisionEnter(Collider* pOther)
{
    pOther->GetOwner()->m_pEntity->dead = true;
    m_pEntity->dead = true;
}