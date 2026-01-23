#include "Missile.h"

inline Missile::Missile() : SphereCollider(XMFLOAT3(1.0f, 1.0f, 1.0f), 1.0f), m_pEmitter(nullptr)
{
    m_firstPos = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

inline Missile::~Missile()
{
    GameObject::~GameObject();
}

inline void Missile::Init(cpu_transform& _transform, Player& _player)
{
    m_pOwnerPlayer = &_player;
    
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

    m_pEmitter = cpuEngine.CreateParticleEmitter();
    m_pEmitter->density = 100.0f;
    m_pEmitter->colorMin = cpu::ToColor(255, 0, 0);
    m_pEmitter->colorMax = cpu::ToColor(255, 128, 0);
}

inline void Missile::Update()
{
    float dt = cpuTime.delta;
    
    XMFLOAT3 pos = m_pEntity->transform.pos;
    float dist = sqrt(pow(pos.x - m_firstPos.x, 2) + pow(pos.z - m_firstPos.z, 2));
    if (dist > 25.f)
        Dead();
    
    m_pEntity->transform.Move(m_speed * dt);
    
    m_pEmitter->pos = m_pEntity->transform.pos;
    m_pEmitter->dir = m_pEntity->transform.dir;
    m_pEmitter->dir.x = -m_pEmitter->dir.x; 
    m_pEmitter->dir.y = -m_pEmitter->dir.y; 
    m_pEmitter->dir.z = -m_pEmitter->dir.z;
    
    m_center = m_pEntity->transform.pos;
}

inline void Missile::OnCollisionEnter(Collider* pOther)
{
    pOther->GetOwner()->m_pEntity->dead = true;
    Dead();
}

inline void Missile::Dead(float _dt)
{
    m_pEntity->dead = true;
    m_pEmitter->dead = true;
}