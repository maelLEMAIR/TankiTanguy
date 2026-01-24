#pragma once
#include "pch.h"
#include "Weapon.h"
#include "GameCamera.hpp"
#include "Life.hpp"
#include "Core/GameObject.h"
#include "Core/SphereCollider.h"

class GameManager;
class Weapon;

class Player : public GameObject, public SphereCollider
{
public:
    Player(bool _isServer);
    ~Player() override;
    void Init(XMFLOAT3 _pos, float _angle, std::string const& _pseudo, XMINT3 _color = XMINT3(255.0f, 255.0f,255.0f));
    
    void Update() override;
    void Dead(float _dt) override;
    
    void Shoot() { m_pWeapon->Shoot(); };
    void AttachCamera();
    void Revive(XMFLOAT3 _pos);
    void SetPseudo(std::string const& _pseudo) { m_pseudo = _pseudo; };

    
    bool IsDead() { return m_life.IsDead(); }
    Life& GetLife() { return m_life; }
    std::string& GetPseudo() { return m_pseudo; }

protected:

private:
    std::string m_pseudo;
    Life m_life;
    
    Weapon* m_pWeapon = nullptr;
    
    GameCamera* m_pCamera = nullptr;
    
    cpu_entity* m_pCanon = nullptr;
    cpu_entity* m_pHead = nullptr;
    
    cpu_transform m_transformCanon;
    
    float m_deadDuration = 0.0f;
    float m_deadTime = 3.0f;
};

