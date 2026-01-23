#pragma once
#include "pch.h"

class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject();
    
    virtual void Update() = 0;
    virtual void Dead(float _dt) = 0;

    std::string const& GetTag() { return m_tag; }

    cpu_entity* m_pEntity = nullptr;
    
protected:
    std::string m_tag;
    
private:
    cpu_mesh m_pMesh;
    cpu_material m_pMaterial;
};

inline GameObject::~GameObject()
{
    m_pEntity = cpuEngine.Release(m_pEntity);
}
