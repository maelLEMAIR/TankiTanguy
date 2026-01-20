#include "SphereCollider.h"
#include "GameObject.h"
#include "GameObject.hpp"
#include "PhysicsComponent.h"
#include "Utils.hpp"

using namespace Utils;

SphereCollider::SphereCollider(XMFLOAT3 _centre, float _radius) : m_centre(_centre), m_radius(_radius)
{}

/////////////////////////////////////////////////////
/// @brief Sphere to Sphere collision resolution
/////////////////////////////////////////////////////
void SphereCollider::RepulseSphere(SphereCollider* o)
{
    float dt = cpuTime.delta;
    if (dt == 0.0f) dt = 0.001f;

    // Adapts the calculations to the Physics Components using the next frame
    XMFLOAT3 pos = m_centre;
    XMFLOAT3 oPos = o->m_centre;
    PhysicsComponent* pC = dynamic_cast<PhysicsComponent*>(m_pOwner);
    PhysicsComponent* opC = dynamic_cast<PhysicsComponent*>(o->m_pOwner);
    if (pC != nullptr)
        pos += pC->m_velocity * dt;
    if (opC != nullptr)
        oPos += opC->m_velocity * dt;

    // Calculate the push
    XMFLOAT3 dir;
    dir.x = pos.x - oPos.x;
    dir.y = pos.y - oPos.y;
    dir.z = pos.z - oPos.z;
    float distance = sqrtf(pow(oPos.x - pos.x, 2) + pow(oPos.y - pos.y, 2) + pow(oPos.z - pos.z, 2));
    dir = (distance > 0) ? dir / distance : XMFLOAT3(1.0f, 0.0f, 1.0f);

    // Apply the push
    XMFLOAT3 push = dir * (m_radius - distance);
    
    if (o->m_rigidBody == true)
    {
        push /= 2.0f;
        if (opC != nullptr)
            opC->m_velocity -= push / dt;
        else
            m_pOwner->m_pEntity->transform.pos.x -= push.x;
        m_pOwner->m_pEntity->transform.pos.y -= push.y;
        m_pOwner->m_pEntity->transform.pos.z -= push.z;
    }
    if (pC != nullptr)
        pC->m_velocity += push / dt;
    else
        m_pOwner->m_pEntity->transform.pos.x += push.x;
        m_pOwner->m_pEntity->transform.pos.y += push.y;
        m_pOwner->m_pEntity->transform.pos.z += push.z;
}

////////////////////////////////////////////////////////////
/// @brief Chooses the correspond collision detection method
////////////////////////////////////////////////////////////
bool SphereCollider::IsColliding(Collider* pOther)
{
    SphereCollider* oS = dynamic_cast<SphereCollider*>(pOther);
    if (oS != nullptr) return SphereToSphere(oS);

    return false;
    //TODO: BoxCollider
}

/////////////////////////////////////////////////////
/// @brief Sphere to Sphere collision detection
/////////////////////////////////////////////////////
bool SphereCollider::SphereToSphere(SphereCollider* pOther)
{
    SphereCollider* o = pOther;
    if (o == nullptr) return false;

    // Adapts the calculations to the Physics Components using the next frame
    XMFLOAT3 pos = m_centre;
    XMFLOAT3 oPos = o->m_centre;
    PhysicsComponent* casted = dynamic_cast<PhysicsComponent*>(GetOwner());
    PhysicsComponent* oCasted = dynamic_cast<PhysicsComponent*>(o->GetOwner());
    if (casted != nullptr)
        pos += casted->m_velocity;
    if (oCasted != nullptr)
        oPos += oCasted->m_velocity;

    // Calculates the distance between the 2 spheres
    float distance = sqrt(pow(pos.x - oPos.x, 2) + pow(pos.y - oPos.y, 2) + pow(pos.z - oPos.z, 2));

    return (distance < m_radius + o->m_radius);
}

