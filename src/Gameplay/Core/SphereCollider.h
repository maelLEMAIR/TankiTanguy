#pragma once
#include "pch.h"
#include "Collider.h"

class BoxCollider;

class SphereCollider : public Collider
{
public:
    SphereCollider(XMFLOAT3 _centre, float _radius);

    void RepulseSphere(SphereCollider* pOther) override;
    
    bool IsColliding(Collider* pOther) override;
    bool SphereToSphere(SphereCollider* pOther);
    bool SphereToBox(BoxCollider* pOther);
    
protected:
    XMFLOAT3 m_centre;
    float m_radius;
    
private:
    friend class BoxCollider;
    

};