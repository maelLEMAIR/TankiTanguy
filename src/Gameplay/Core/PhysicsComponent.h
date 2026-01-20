#pragma once
#include "pch.h"

class GameObject;

class PhysicsComponent
{
public:
    PhysicsComponent();
    ~PhysicsComponent();

    enum class Force
    {
        IMPULSE, FORCE
    };
    
    void AddForce(XMFLOAT3 const& force, Force forceType);
    
protected:
    float m_speed = 0.0f;
    XMFLOAT3 m_direction;
    XMFLOAT3 m_velocity;

    bool m_useGravity = true;
    float m_gravity = -0.981f;
    float m_mass = 1.0f;

    int m_prioritizedAxis = -1;
    
    bool m_useMaxVelocityGlobal  = false;
    bool m_useMaxVelocityX       = false;
    bool m_useMaxVelocityY       = false;
    bool m_useMaxVelocityZ       = false;

    XMFLOAT3 m_maxVelocities;
    float m_globalMaxVelocity = 10.0f;

    GameObject* m_pOwnerPhysics = nullptr;
    
private:
    std::vector<std::pair<XMFLOAT3, Force>> m_appliedForces;
    XMFLOAT3 m_previousVelocity;
    
    friend class PhysicsSystem;
    friend class SphereCollider;
    friend class Collider;
};