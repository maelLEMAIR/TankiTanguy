#include "GameManager.h"

#include <random>

#include "Collider.h"
#include "Utils.hpp"
#include "Client/App.h"

GameManager* GameManager::s_pInstance = nullptr;

GameManager::GameManager() : m_pPlCrt(nullptr)
{
    m_map = Map();
    s_pInstance = this;
}

GameManager::~GameManager()
{
    m_gameObjects.clear();
    for (Player* player : m_players)
        delete player;
    m_players.clear();
    delete m_pPlCrt;
    s_pInstance = nullptr;
}

GameManager& GameManager::Get()
{
    return *s_pInstance;
}

void GameManager::Start()
{
    delete m_pPlCrt;
    m_pPlCrt = new PlayerController();

    m_map.Init(50, 50);

    // Player
    AddPlayer(XMFLOAT3(0, 0, 0), 0.0f, XMINT3(255, 0, 0), "rouge", true);
    AddPlayer(XMFLOAT3(0, 0, 25), XM_PI, XMINT3(0, 0, 255), "bleu");
    AddPlayer(XMFLOAT3(12.5, 0, 12.5), -XM_PIDIV2, XMINT3(0, 255, 0), "vert");

    m_pFloorMesh.CreateCube();
    m_pFloorMaterial.color = cpu::ToColor(14, 74, 16);
	
    for (int i = -(m_map.m_width / 2); i < m_map.m_width; i++)
    {
        for (int j = -(m_map.m_height / 2); j < m_map.m_height; j++)
        {
            cpu_entity* floor = cpuEngine.CreateEntity();
            floor->pMesh = &m_pFloorMesh;
            floor->pMaterial = &m_pFloorMaterial;
            floor->transform.pos = XMFLOAT3(static_cast<float>(j) * 5, -1.0f, static_cast<float>(i) * 5);
            floor->transform.sca = XMFLOAT3(5.0f, 1.0f, 5.0f);
            m_floors.push_back(floor);
        }
    }
}

void GameManager::Update()
{
    // float dt = cpuTime.delta;
    CollisionCheck();
    m_pPlCrt->HandleInput();
    for (GameObject* go : m_gameObjects)
        go->Update();
}

bool GameManager::WorldToScreen( XMFLOAT3 const& world, XMFLOAT2& screen, XMFLOAT4X4 const& viewProj, int screenWidth, int screenHeight)
{
    float clip[4];

    clip[0] = world.x * viewProj.m[0][0] + world.y * viewProj.m[1][0] + world.z * viewProj.m[2][0] + viewProj.m[3][0];
    clip[1] = world.x * viewProj.m[0][1] + world.y * viewProj.m[1][1] + world.z * viewProj.m[2][1] + viewProj.m[3][1];
    clip[2] = world.x * viewProj.m[0][2] + world.y * viewProj.m[1][2] + world.z * viewProj.m[2][2] + viewProj.m[3][2];
    clip[3] = world.x * viewProj.m[0][3] + world.y * viewProj.m[1][3] + world.z * viewProj.m[2][3] + viewProj.m[3][3];

    if (clip[3] <= 0.0f)
        return false;

    float ndcX = clip[0] / clip[3];
    float ndcY = clip[1] / clip[3];

    screen.x = (ndcX * 0.5f + 0.5f) * static_cast<float>(screenWidth);
    screen.y = (1.0f - (ndcY * 0.5f + 0.5f)) * static_cast<float>(screenHeight);

    return true;
}

void GameManager::CollisionCheck()
{
    std::vector<Collider*> colliders = GetAllComponents<Collider>();

    for (Collider* pCollider : colliders)
    {
        if (pCollider->m_isActiveCollider == false)   continue;
        
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

void GameManager::AddPlayer(XMFLOAT3 _pos, float _angle, XMINT3 _color, std::string _pseudo, bool _mainPlayer)
{
    Player* player = CreateObject<Player>();
    player->Init(_pos, _angle, _color, _pseudo);
    if (_mainPlayer)
    {
        player->AttachCamera();
        m_pPlCrt->Init(*player);
    }
    m_players.push_back(player);
}

void GameManager::RespawnPlayer(Player* _player, float _minDistanceBetweenPlayers)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, m_map.m_map.size() - 1);
    constexpr int MAX_ATTEMPTS = 100;
    int attempts = 0;

    while (attempts < MAX_ATTEMPTS)
    {
        attempts++;
        
        XMINT2 tile = m_map.m_map[dist(gen)];
        XMFLOAT2 spawnPos = { (float)tile.x, (float)tile.y };

        bool valid = true;
        for (Player* other : m_players)
        {
            if (other == _player)
                continue;
            
            if (other->IsDead())
                continue;

            XMFLOAT2 otherPos = XMFLOAT2(other->GetPos().x, other->GetPos().z);
            float d = Utils::Distance(spawnPos, otherPos);
            if (d < _minDistanceBetweenPlayers)
            {
                valid = false;
                break;
            }
        }

        if (valid)
        {
            _player->Revive(XMFLOAT3(spawnPos.x, 0.0f,  spawnPos.y));
            return;
        }
    }
    
    // Fallback si aucune position trouvée
    _player->Revive({ 0.f, 0.f, 0.f });
}

