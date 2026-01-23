#pragma once
#include "Weapon.h"
#include "GameCamera.hpp"
#include "Life.hpp"
#include "pch.h"
#include "Core/GameObject.hpp"
#include "Core/SphereCollider.h"

class PlayerManager;
class Weapon;

class Player : public GameObject, public SphereCollider
{
public:
    Player();
    ~Player() override;
    void Init(XMFLOAT3 _pos, float _angle, XMINT3 _color, std::string const& _pseudo);
    
    void Update() override;
    void Dead(float _dt) override;
    
    void Move(float _dir);
    void Turn(float _dir);
    void Shoot() { m_pWeapon->Shoot(); };
    void Revive(XMFLOAT3 _pos);
    void AttachCamera();
    
    bool IsDead() { return m_life.IsDead(); }
    XMFLOAT3& GetPos() {return m_pEntity->transform.pos; }
    
    Life& GetLife() { return m_life; }
    XMFLOAT3 GetPos() const { return m_pEntity->transform.pos; }
    std::string& GetPseudo() { return m_pseudo; }

protected:

private:
    cpu_entity* m_pHead;
    cpu_entity* m_pCanon;
    
    cpu_mesh m_meshTank;
    cpu_material m_materialTank;

    std::string m_pseudo;
    Life m_life;
    
    GameCamera* m_pCamera;
    Weapon* m_pWeapon;
    float m_speed = 2.5f;

    float m_pAngle = XM_PI;
    
    float m_deadDuration = 0.0f;
    float m_deadTime = 3.0f;
};

