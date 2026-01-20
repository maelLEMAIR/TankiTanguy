#pragma once
#include "pch.h"

class BoxCollider;
class SphereCollider;
class GameObject;

class Collider 
{
public:
    Collider() = default;
    virtual ~Collider() = default;

    GameObject* GetOwner() { return m_pOwner; }

    virtual bool IsColliding(Collider* pOther) = 0;
    virtual void Repulse    (Collider* pOther);
    
    virtual void OnCollisionEnter(Collider* pOther);
    virtual void OnCollisionStay (Collider* pOther);
    virtual void OnCollisionExit (Collider* pOther);
    
protected:
    GameObject* m_pOwner = nullptr;
    bool m_isActiveCollider = true;
    
    bool m_rigidBody = true;
    bool m_isTrigger = false;
    std::vector<Collider*> m_collidingEntities;
    bool IsAlreadyColliding(Collider* pOther);
    
    virtual void RepulseBox    (BoxCollider* pOther)    = 0;
    virtual void RepulseSphere (SphereCollider* pOther) = 0;

private:
    
    friend class PhysicsSystem;
};

#endif