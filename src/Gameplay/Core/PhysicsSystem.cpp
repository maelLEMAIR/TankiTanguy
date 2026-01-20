#include "pch.h"

#include "PhysicsSystem.h"
#include "Utils.hpp"
#include "GameManager.h"
#include "Collider.h"
#include "PhysicsComponent.h"

//////////////////////////////////////////////////////
/// @brief Update PhysicsComponents and apply forces
/////////////////////////////////////////////////////
void PhysicsSystem::PhysicsUpdate(float deltaTime)
{
    std::vector<PhysicsComponent*> physicsComponents = GameManager::Get()->GetCurrentScene()->GetAllComponents<PhysicsComponent>();

    for (PhysicsComponent* pC : physicsComponents)
    {
        if (pC->m_pOwnerPhysics->IsActive() == false) continue;
        
        pC->m_previousVelocity = pC->m_velocity;

        // Gravity 
        if (pC->m_useGravity)
            pC->AddForce(XMFLOAT3(0.0f, 1.0f, 0.0f) * pC->m_gravity * pC->m_mass, PhysicsComponent::Force::FORCE);

        // Calculate current frame applied forces
        for (std::pair<XMFLOAT3, PhysicsComponent::Force> pair : pC->m_appliedForces )
        {
            XMFLOAT3 force = pair.first;
            PhysicsComponent::Force type = pair.second;

            if (type == PhysicsComponent::Force::FORCE)
                pC->m_velocity += (force / pC->m_mass) * deltaTime;
            if (type == PhysicsComponent::Force::IMPULSE)
                pC->m_velocity += force / pC->m_mass;
        }
        
        pC->m_direction = Utils::Normalize(pC->m_velocity);
        pC->m_speed = Utils::Length(pC->m_direction);

        // Clamp velocity to set limits
        if (pC->m_useMaxVelocityGlobal)
        {
            pC->m_velocity.x = cpu::Clamp(pC->m_velocity.x, -pC->m_globalMaxVelocity, pC->m_globalMaxVelocity);
            pC->m_velocity.y = cpu::Clamp(pC->m_velocity.y, -pC->m_globalMaxVelocity, pC->m_globalMaxVelocity);
            pC->m_velocity.z = cpu::Clamp(pC->m_velocity.z, -pC->m_globalMaxVelocity, pC->m_globalMaxVelocity);
        }
        if (pC->m_useMaxVelocityX)
            pC->m_velocity.x = cpu::Clamp(pC->m_velocity.x, -pC->m_maxVelocities.x, pC->m_maxVelocities.x);
        if (pC->m_useMaxVelocityY)
            pC->m_velocity.y = cpu::Clamp(pC->m_velocity.y, -pC->m_maxVelocities.y, pC->m_maxVelocities.y);
        if (pC->m_useMaxVelocityZ)
            pC->m_velocity.z = cpu::Clamp(pC->m_velocity.z, -pC->m_maxVelocities.z, pC->m_maxVelocities.z);
        
        pC->m_appliedForces.clear();
    }

    CollisionCheck();

    // Apply the calculated forces
    for (PhysicsComponent* pC : physicsComponents)
    {
        if (pC->m_pOwnerPhysics == nullptr) continue;
        pC->m_pOwnerPhysics->m_transform.Translate(pC->m_velocity * deltaTime);
    }
}

//////////////////////////////////////////////////////
/// @brief Handle collision detection and resolution
/////////////////////////////////////////////////////
void PhysicsSystem::CollisionCheck()
{
    std::vector<Collider*> colliders = GameManager::Get()->GetCurrentScene()->GetAllComponents<Collider>();

    for (Collider* pCollider : colliders)
    {
        if (pCollider->m_isActiveCollider == false)   continue;
        if (pCollider->m_pOwner->IsActive() == false) continue;
        
        for (Collider* pOther : colliders)
        {
            if (pCollider == pOther) continue;
            if (pOther->m_pOwner->IsActive() == false) continue;
            
            if (pCollider->IsColliding(pOther))
            {
                if (pCollider->IsAlreadyColliding(pOther) == false)
                    pCollider->OnCollisionEnter(pOther);
                if (pCollider->IsAlreadyColliding(pOther) == true)
                    pCollider->OnCollisionStay(pOther);
            }
            else
            {  
                if (pCollider->IsAlreadyColliding(pOther) == true)  pCollider->OnCollisionExit(pOther);
            }
        }
    }
}