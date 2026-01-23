#pragma once
#include "pch.h"

#include <iostream>

#include "GameCamera.hpp"
#include "Weapon.h"
#include "Core/GameObject.hpp"
#include "Core/SphereCollider.h"

class Player : public GameObject, public SphereCollider
{
public:
    Player();
    ~Player() override;

    void Init(XMFLOAT3 _pos, float _angle, XMINT3 _color, std::string _pseudo);
    void ServerInit(XMFLOAT3 _pos, float _angle, XMINT3 _color, std::string _pseudo);

    void Update();

    void Move(float _dir);
    void Turn(float _dir);
    void Shoot() { m_pWeapon->Shoot(); };

    XMFLOAT3 GetPos() const { return m_pEntity->transform.pos; }
    std::string& GetPseudo() { return m_pseudo; }
	float GetAngle() const { return m_pAngle; }

    void SetPos(XMFLOAT3 pos)  { m_pEntity->transform.pos = pos; }
	void SetAngle(float angle) { m_pAngle = angle; }
protected:

private:
    cpu_entity* m_pHead;
    cpu_entity* m_pCanon;
    
    cpu_mesh m_meshTank;
    cpu_material m_materialTank;

    std::string m_pseudo;
    
    GameCamera* m_pCamera;
    Weapon* m_pWeapon;
    float m_speed = 2.5f;

    float m_pAngle = XM_PI;
};

inline Player::Player() : SphereCollider(XMFLOAT3(), 1.0f), m_pHead(nullptr), m_pCanon(nullptr), m_pCamera(nullptr), m_pWeapon(nullptr) { }

inline void Player::Init(XMFLOAT3 _pos, float _angle, XMINT3 _color, std::string _pseudo)
{
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
    
    m_pCamera = new GameCamera(m_pEntity->transform);
    m_pWeapon = new Weapon(m_pCanon->transform);
}

inline void Player::ServerInit(XMFLOAT3 _pos, float _angle, XMINT3 _color, std::string _pseudo)
{
    m_pseudo = _pseudo;
    m_pAngle = _angle;
    m_pEntity->transform.SetYPR(m_pAngle, 0.f, 0.f);
    m_pEntity->transform.sca = XMFLOAT3(1.0f, 1.0f, 2.0f);

    m_center = _pos;
    m_pOwner = this;
    m_rigidBody = false;

    m_pHead = cpuEngine.CreateEntity();
    m_pHead->transform.pos = _pos;
    m_pHead->transform.pos.y += 0.5f;
    m_pHead->transform.sca = XMFLOAT3(0.75f, 0.5f, 0.75f);

    XMFLOAT3 forward = m_pEntity->transform.dir;

    m_pCanon = cpuEngine.CreateEntity();
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

    m_pCamera = new GameCamera(m_pEntity->transform);
    m_pWeapon = new Weapon(m_pCanon->transform);
}

inline Player::~Player()
{
    m_pHead = cpuEngine.Release(m_pHead);
    m_pCanon = cpuEngine.Release(m_pCanon);
    CPU_DELPTR( m_pWeapon );
    CPU_DELPTR( m_pCamera );
}

inline void Player::Update()
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
    
    m_pCamera->Update();
    m_pWeapon->Update();

    m_center = posBody;
}

inline void Player::Move(float _dir)
{
    float dt = cpuTime.delta;
    m_pEntity->transform.Move(dt * m_speed * _dir);
}

inline void Player::Turn(float _dir)
{
    float dt = cpuTime.delta;
    m_pAngle += _dir * dt;
    m_pEntity->transform.AddYPR(dt * _dir);
}
