#pragma once
#include "pch.h"

class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject() = default;
    
    cpu_entity* m_pEntity = nullptr;
protected:
    
private:
    cpu_mesh m_pMesh;
    cpu_material m_pMaterial;
};