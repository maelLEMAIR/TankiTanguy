#include "GameObject.h"

GameObject::GameObject(int isServer)
{
    m_isServer = isServer;
    if (m_isServer == false)
    {
        m_pEntity = cpuEngine.CreateEntity();
    }
}

void GameObject::Update()
{
    float dt = cpuTime.delta;
    
}

void GameObject::Move(float _dir)
{
    float dt = cpuTime.delta;
    if (m_isServer)
        m_transform.Move(_dir * dt * m_speed);
    else
        m_pEntity->transform.Move(_dir * dt* m_speed);
}

void GameObject::Turn(float _dir)
{
    float dt = cpuTime.delta;
    if (m_isServer)
        m_transform.AddYPR(_dir * dt);
    else
        m_pEntity->transform.AddYPR(_dir * dt);
}

XMFLOAT3& GameObject::GetPos()
{
    if (m_isServer)
        return m_transform.pos;

    return m_pEntity->transform.pos;
}
