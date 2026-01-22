#include "GameManager.h"

#include "Collider.h"
#include "GameObject.hpp"
#include "Client/App.h"
#include <Network/Network.hpp>
#include "PlayerController.hpp"

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

void GameManager::ClientSideStart()
{
	m_pLocalPlayerCtrl = new PlayerController();
	m_pLocalPlayerCtrl->Init(*App::GetInstance().m_pPlayer);
}

void GameManager::ClientSideUpdate()
{
    float dt = cpuTime.delta;
	m_pLocalPlayerCtrl->UpdateInputs();
}

void GameManager::ClientSideStop()
{
    delete s_pInstance;
	s_pInstance = nullptr;
}

void GameManager::ServerSideStart()
{
    m_matchState.players[0].player = new Player();
    m_matchState.players[0].player->ServerInit(XMFLOAT3(0, 0, 0), 0.0f, XMINT3(255, 0, 0), "rouge");
    m_matchState.players[0].playerCtrl = new PlayerController();
    m_matchState.players[0].playerCtrl->Init(*m_matchState.players[0].player);

    m_matchState.players[1].player = new Player();
    m_matchState.players[1].player->ServerInit(XMFLOAT3(0, 0, 25), XM_PI, XMINT3(0, 0, 255), "bleu");
    m_matchState.players[1].playerCtrl = new PlayerController();
    m_matchState.players[1].playerCtrl->Init(*m_matchState.players[1].player);
}

void GameManager::ServerSideUpdate()
{
    float dt = cpuTime.delta;
    CollisionCheck();
    UpdateMatchState();
}

void GameManager::ServerSideStop()
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

void GameManager::UpdateMatchState()
{
    for (PlayerState& playerState : m_matchState.players)
    {
		PlayerController* playerCtrl = playerState.playerCtrl;
        for (int input : playerState.inputs)
        {
			playerCtrl->HandleInput(input);
		}
        playerState.inputs.clear();
		playerState.player->Update();
	}

    for ( Missile* missile : m_matchState.missiles)
    {
        missile->Update();
	}
}

