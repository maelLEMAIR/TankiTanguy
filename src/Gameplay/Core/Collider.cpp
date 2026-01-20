#include "Collider.h"
#include "GameObject.hpp"
#include "SphereCollider.h"

bool Collider::IsAlreadyColliding(Collider* pOther)
{
    for (Collider* pCollider : m_collidingEntities)
    {
        if (pCollider == pOther) return true;
    }
    return false;
}

void Collider::Repulse(Collider* pOther)
{
    SphereCollider* sphereCast = dynamic_cast<SphereCollider*>(pOther);
    if ((sphereCast == nullptr) == false) RepulseSphere(sphereCast);

    //TODO: BoxCollider
}

void Collider::OnCollisionEnter(Collider* pOther)
{
    m_collidingEntities.push_back(pOther);   
}

void Collider::OnCollisionStay(Collider* pOther)
{
    if (pOther->m_isTrigger == true)
        return;
    if (m_rigidBody == true)
        Repulse(pOther);
}

void Collider::OnCollisionExit(Collider* pOther)
{
    for (int i = 0; i < m_collidingEntities.size(); i++)
    {
        m_collidingEntities.erase(m_collidingEntities.begin() + i);
    }
}