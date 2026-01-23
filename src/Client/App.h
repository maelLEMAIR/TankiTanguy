#pragma once

#include "InputText.h"

constexpr size_t MAX_UDP_PACKET_SIZE = 1500;

class ClientSocket;
class GameManager;
class Player;
class PlayerController;

class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void OnStart();
	void OnUpdate();
	void OnExit();
	void OnRender(int pass);
	bool WorldToScreen(XMFLOAT3 const& world, XMFLOAT2& screen, XMFLOAT4X4 const& viewProj, int screenWidth, int screenHeight);
	static DWORD WINAPI Listen(LPVOID lpParam);

	Player* m_pPlayer;
	
private:
	inline static App* s_pApp = nullptr;
	GameManager* m_pGameManager = nullptr;
	
	// Resources
	cpu_font m_font;
	cpu_rt* m_rts[1];

	// 3D
	Player* m_pPlayer2;
	Player* m_pPlayer3;

	// UI
	InputText* m_pseudo;
	InputText* m_loginAdrr;
	InputText* m_loginPort;

	//Network
	ClientSocket* m_pClientSocket;
};
