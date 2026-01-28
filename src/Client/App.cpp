#include "pch.h"
#include "Core/UIButton.h"
#include "Core/UIDialogue.h"
#include "Gameplay/Player.h"
#include "Core/UIHealthBar.hpp"
#include "Core/UIManager.h"
#include "Core/UIText.hpp"

App::App() : m_gameManager(GameManager(false))
{
	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);

	m_pUIManager = UIManager::Get();
}

App::~App()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::OnStart()
{
	// Resources
	m_font.Create(12);
	m_rts[0] = cpuEngine.CreateRT();
	
	m_gameManager.Start();
	cpuEngine.GetParticleData()->Create(1000000);

	// UI
	XMINT2 pos = XMINT2(cpuDevice.GetWidth() / 2, 30);
	m_dialogueLog = new UIDialogue();
	m_dialogueLog->AddInputText(m_font, pos, CPU_TEXT_CENTER, "Enter an address: ", m_addr);
	m_dialogueLog->AddInputText(m_font, pos, CPU_TEXT_CENTER, "Enter a port: ", m_port);
	m_dialogueLog->AddInputText(m_font, pos, CPU_TEXT_CENTER, "Enter a pseudo: ", m_pseudo);
	
	m_pErrorText = m_pUIManager->Create<UIText>();
	m_pErrorText->Init(XMINT2(cpuDevice.GetWidth() / 2, cpuDevice.GetHeight() / 2 + 50), CPU_TEXT_LEFT, "");
	m_pErrorText->Create(10, CPU_RED);
	m_pErrorText->SetActive(false);
	
	cpuEngine.GetCamera()->transform.pos = XMFLOAT3(100.0f, 100.0f, 100.0f);
	
	/*UIButton* startBtn = m_pUIManager->Create<UIButton>();
	startBtn->Init(XMINT2(cpuDevice.GetWidth() / 2, cpuDevice.GetHeight() / 2), XMINT2(24, 24), pathGreen, pathGray);
	
	startBtn->SetOnClickCallback([startBtn]() {
		int i = 0;
		startBtn->SetToBeDestroyed();
	});*/
	
	m_pFloorMesh.CreateCube();
	m_pFloorMaterial.color = cpu::ToColor(14, 74, 16);
	for (int i = -(m_width / 2); i < m_width; i++)
	{
		for (int j = -(m_height / 2); j < m_height; j++)
		{
			cpu_entity* floor = cpuEngine.CreateEntity();
			floor->pMesh = &m_pFloorMesh;
			floor->pMaterial = &m_pFloorMaterial;
			floor->transform.pos = XMFLOAT3(static_cast<float>(j) * 5, -1.0f, static_cast<float>(i) * 5);
			floor->transform.sca = XMFLOAT3(5.0f, 1.0f, 5.0f);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsValidPort(const std::string& str, int& outPort)
{
	if (str.empty())
		return false;

	for (char c : str)
	{
		if (!std::isdigit(static_cast<unsigned char>(c)))
			return false;
	}
	
	outPort = std::stoi(str);
	return outPort > 0 && outPort <= 65535;
}

void App::OnUpdate()
{
	// Quit
	if ( cpuInput.IsKeyDown(VK_ESCAPE) )
		cpuEngine.Quit();
	
	if (m_isLogin == false)
		Log();

	if (m_isRequested)
	{
		if (m_client.m_isConnected)
		{
			m_isRequested = false;
			m_isLogin = true;
			m_requestProgress = 0.0f;
			
			m_id = m_client.GetServerInfo().playerId;
			m_gameManager.SetMainPlayer(m_id);
			m_pPlayer = m_gameManager.GetMainPlayer();
			m_pPlayer->SetPseudo(m_pseudo);
			SetUI();
			return;
		}
		if ( m_requestProgress >= m_requestTime )
		{
			m_isRequested = false;
			m_dialogueLog->Reset();
			m_pseudo = "Connexion Failed ! ";
			m_requestProgress = 0.0f;
			return;
		}
		m_requestProgress += cpuTime.delta;
	}
	
	if (m_isLogin == false)
		return;

	// 10.10.137.53 Baptiste
	// 10.10.137.66 Auxence
	// 10.10.137.11 Vivien
	
	// Update
	m_pUiScore->SetText(m_pPlayer->GetScore());
	m_pUIManager->Update();
	m_gameManager.Update();
}

void App::SetUI()
{
	std::string pathGreen = "../../res/Gameplay/vert.png";
	std::string pathGray = "../../res/Gameplay/gris.png";
	
	UIHealthBar* uiBar = m_pUIManager->Create<UIHealthBar>();
	uiBar->Init(m_font, XMINT2(0.0f, cpuDevice.GetHeight() - 24), 24.0f, *m_pPlayer, pathGreen, pathGray);

	m_pUiScore = m_pUIManager->Create<UIText>();
	m_pUiScore->Init(XMINT2(cpuDevice.GetWidth() - 10, 5), CPU_TEXT_RIGHT, m_pPlayer->GetScore());
	m_pUiScore->SetFont(m_font);
	
	m_pRespawnText = m_pUIManager->Create<UIText>();
	m_pRespawnText->Init(XMINT2(cpuDevice.GetWidth() / 2, cpuDevice.GetHeight() / 2), CPU_TEXT_CENTER, "");
	m_pRespawnText->Create(12, CPU_GREEN);
}

void App::Log()
{
	if (m_isRequested == false)
	{
		m_dialogueLog->Update();
		if (m_dialogueLog->IsActive() == false)
		{
			int port;
			if (IsValidPort(m_port, port) == false)
			{
				m_isRequested = false;
				m_dialogueLog->Reset();
				m_pErrorText->SetText("Error Port");
				return;
			}
			
			if (m_client.ConnectToServer(m_addr.c_str(), std::stoi(m_port), m_pseudo.c_str()))
			{
				m_isRequested = true;
				m_pPseudo->SetText(m_pseudo);
				return;
			}
			m_isRequested = false;
			m_dialogueLog->Reset();
			m_pErrorText->SetText("Wrong Addresse or Port");
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::OnExit()
{
	//CPU_DELPTR(m_pseudo);
	CPU_DELPTR(m_pUIManager);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::OnRender(int pass)
{
	// YOUR CODE HERE
	m_pUIManager->Render();
	
	std::string info;
	XMFLOAT2 screenPos;
	std::vector<Player*> players = m_gameManager.GetPlayers();
		
	if (m_isLogin == false)
		m_dialogueLog->Render();
		
	for (Player* player: players)
	{
		if (player == m_pPlayer)
			continue;

		if (player->IsDead())
			continue;
		
		if (m_gameManager.WorldToScreen(player->GetPos(), screenPos, cpuEngine.GetCamera()->matViewProj, cpuDevice.GetWidth(), cpuDevice.GetHeight()))
			cpuDevice.DrawText(&m_font, player->GetPseudo().c_str(), screenPos.x, screenPos.y - 25.0f, CPU_TEXT_CENTER );
	}

	if (m_pPlayer->IsDead())
	{
		m_pRespawnText->SetActive(true);
		m_respawnTime -= cpuTime.delta;
		std::string respawn = "Respawn in..." + std::to_string(static_cast<int>(m_respawnTime));
		m_pRespawnText->SetText(respawn);
	}
	else
		m_respawnTime = 3.0f;
	
	cpuDevice.DrawText(&m_font, "TANKYTANGUY", static_cast<int>((cpuDevice.GetWidth() * 0.5f)), 10, CPU_TEXT_CENTER);
	cpuDevice.DrawText(&m_font, info.c_str(), static_cast<int>((cpuDevice.GetWidth() * 0.5f)), 30, CPU_TEXT_CENTER);

	if (m_pseudo != "")
		m_pPseudo->SetActive(true);

}
