#pragma once
#include "pch.h"

class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject();
    virtual void Update() = 0;
    
    cpu_entity* m_pEntity = nullptr;
protected:
    
private:
    cpu_mesh m_pMesh;
    cpu_material m_pMaterial;
};

inline GameObject::~GameObject()
{
    m_pEntity = cpuEngine.Release(m_pEntity);
}
