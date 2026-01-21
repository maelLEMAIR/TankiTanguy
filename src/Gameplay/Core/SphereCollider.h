#pragma once
#include "pch.h"
#include "Collider.h"

class BoxCollider;

class SphereCollider : public Collider
{
public:
    SphereCollider(XMFLOAT3 _center, float _radius);

    void RepulseSphere(SphereCollider* pOther) override;
    
    bool IsColliding(Collider* pOther) override;
    bool SphereToSphere(SphereCollider* pOther);
    
protected:
    XMFLOAT3 m_center;
    float m_radius;
    
private:
    friend class BoxCollider;
    

};