#pragma once
#include "pch.h"
#include "Core/GameObject.hpp"
#include "Core/SphereCollider.h"

class Player;

class Missile : public GameObject, public SphereCollider
{
public:
    Missile();
    ~Missile() override;
    void Init(cpu_transform& _transform, Player& _player);
    void Update() override;
    void OnCollisionEnter(Collider* pOther) override;
    void Dead(float _dt = 0.0f) override;

    cpu_entity& GetEntity() const {return *m_pEntity; };
    
protected:
    
private:
    XMFLOAT3 m_firstPos;
    
    cpu_mesh m_meshMissile;
    cpu_material m_missileMaterial;
    cpu_particle_emitter* m_pEmitter;

    Player* m_pOwnerPlayer = nullptr;
    
    float m_speed = 10.0f;
};