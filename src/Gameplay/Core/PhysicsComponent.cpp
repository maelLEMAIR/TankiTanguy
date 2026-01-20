#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent() :
    m_direction(), m_velocity(), m_maxVelocities(), m_previousVelocity()
{
    
}

PhysicsComponent::~PhysicsComponent()
{
    
}

void PhysicsComponent::AddForce(XMFLOAT3 const& force, Force forceType)
{
    m_appliedForces.push_back(std::make_pair(force, forceType));
}