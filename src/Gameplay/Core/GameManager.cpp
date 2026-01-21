#include "GameManager.h"

#include "Collider.h"
#include "GameObject.hpp"
#include "Client/App.h"

GameManager* GameManager::s_pInstance = nullptr;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

GameManager* GameManager::Get()
{
    if (s_pInstance == nullptr) s_pInstance = new GameManager();
    return s_pInstance;
}

void GameManager::Start()
{

}

void GameManager::Update()
{
    float dt = cpuTime.delta;
    CollisionCheck();
}

void GameManager::Stop()
{
    delete s_pInstance;
	s_pInstance = nullptr;
}

void GameManager::CollisionCheck()
{
    std::vector<Collider*> colliders = GetAllComponents<Collider>();

    for (Collider* pCollider : colliders)
    {
        if (pCollider->m_isActiveCollider == false) continue;
        
        for (Collider* pOther : colliders)
        {
            if (pCollider == pOther) continue;
            
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

