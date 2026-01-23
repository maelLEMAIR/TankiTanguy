#include "Player.h"
#include "Weapon.h"

#include "Core/PlayerManager.h"

Player::Player() : SphereCollider(XMFLOAT3(), 1.0f), m_pHead(nullptr), m_pCanon(nullptr), m_pCamera(nullptr), m_pWeapon(nullptr), m_life(Life(100)) { }

Player::~Player()
{
    m_pHead = cpuEngine.Release(m_pHead);
    m_pCanon = cpuEngine.Release(m_pCanon);
    CPU_DELPTR( m_pWeapon );
    CPU_DELPTR( m_pCamera );
}

void Player::Init(XMFLOAT3 _pos, float _angle, XMINT3 _color, std::string const& _pseudo)
{
    m_tag = "Player";
    m_pseudo = _pseudo;
    m_pAngle = _angle;
    
    m_materialTank.color = cpu::ToColor(_color.x, _color.y, _color.z);
    m_meshTank.CreateCube();
    
    m_pEntity->pMesh = &m_meshTank;
    m_pEntity->pMaterial = &m_materialTank;
    m_pEntity->transform.pos = _pos;
    m_pEntity->transform.SetYPR(m_pAngle, 0.f, 0.f);
    m_pEntity->transform.sca = XMFLOAT3(1.0f, 1.0f, 2.0f);

    m_center = _pos;
    m_pOwner = this;
    m_rigidBody = false;
    
    m_pHead = cpuEngine.CreateEntity();
    m_pHead->pMesh = &m_meshTank;
    m_pHead->pMaterial = &m_materialTank;
    m_pHead->transform.pos = _pos;
    m_pHead->transform.pos.y += 0.5f;
    m_pHead->transform.sca = XMFLOAT3(0.75f, 0.5f, 0.75f);

    XMFLOAT3 forward = m_pEntity->transform.dir;
    
    m_pCanon = cpuEngine.CreateEntity();
    m_pCanon->pMesh = &m_meshTank;
    m_pCanon->pMaterial = &m_materialTank;
    m_pCanon->transform.pos = _pos;
    m_pCanon->transform.pos.x += forward.x * 1.5f;
    m_pCanon->transform.pos.z += forward.z * 1.5f;
    m_pCanon->transform.pos.y += 0.5f;
    m_pCanon->transform.sca = XMFLOAT3(0.25f, 0.25f, 3.0f);
    
    m_pCanon->transform.LookAt(
        m_pCanon->transform.pos.x + forward.x,
        m_pCanon->transform.pos.y,
        m_pCanon->transform.pos.z + forward.z
    );
    m_pWeapon = new Weapon(m_pCanon->transform, *this);
}

void Player::Update()
{
    float dt = cpuTime.delta;
    
    m_pEntity->transform.SetYPR(m_pAngle, 0.f, 0.f);
        
    XMFLOAT3 posBody = m_pEntity->transform.pos;
    
    m_pHead->transform.SetYPR(m_pAngle, 0.f, 0.f);
    m_pHead->transform.pos = posBody;
    m_pHead->transform.pos.y += 0.5f;
    
    XMFLOAT3 forward = m_pEntity->transform.dir;

    m_pCanon->transform.pos = posBody;
    m_pCanon->transform.pos.x += forward.x * 1.5f;
    m_pCanon->transform.pos.z += forward.z * 1.5f;
    m_pCanon->transform.pos.y += 0.5f;

    m_pCanon->transform.LookAt(
        m_pCanon->transform.pos.x + forward.x,
        m_pCanon->transform.pos.y,
        m_pCanon->transform.pos.z + forward.z
    );
    
    m_pWeapon->Update();
    m_center = posBody;
    
    if (m_life.IsDead())
    {
        Dead(dt);
        return;
    }
    
    if (m_pCamera != nullptr)
        m_pCamera->FollowPlayer();
    
}

void Player::Move(float _dir)
{
    float dt = cpuTime.delta;
    m_pEntity->transform.Move(dt * m_speed * _dir);
}

void Player::Turn(float _dir)
{
    float dt = cpuTime.delta;
    m_pAngle += _dir * dt;
    m_pEntity->transform.AddYPR(dt * _dir);
}

void Player::Revive(XMFLOAT3 _pos)
{
    m_pEntity->transform.pos = _pos;
    m_life.ResetLIfe();
}

void Player::AttachCamera()
{
    m_pCamera = new GameCamera(m_pEntity->transform);
}

void Player::Dead(float _dt)
{
    if (m_deadDuration > m_deadTime)
    {
        PlayerManager::Get()->RespawnPlayer(this);
        m_deadDuration = 0.0f;
        return;
    }
    m_pEntity->transform.pos.y = -2.0f;
    m_deadDuration += _dt;
}
