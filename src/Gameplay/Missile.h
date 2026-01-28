#pragma once
#include "pch.h"
#include "Core/GameObject.h"
#include "Core/SphereCollider.h"

class Player;
class Score;

class Missile : public GameObject, public SphereCollider
{
public:
    Missile(bool _isServer);
    ~Missile() override;
    void Init(cpu_transform& _transform, Player& _player);
    void Update() override;
    void OnCollisionEnter(Collider* pOther) override;
    void Dead(float _dt = 0.0f);
    int GetShooterId();
    
protected:
    
private:
    XMFLOAT3 m_firstPos;
    Player* m_pOwnerPlayer = nullptr;
    
    cpu_particle_emitter* m_pEmitter;
};