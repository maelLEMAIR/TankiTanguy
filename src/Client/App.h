#pragma once

#include "InputText.h"
#include "Core/UIManager.h"
#include "Gameplay/Player.hpp"
#include "Gameplay/Core/GameManager.h"

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
	bool WorldToScreen(XMFLOAT3 const& world, XMFLOAT2& screen, XMFLOAT4X4 const& viewProj, int screenWidth,
	                   int screenHeight);

	Player* m_pPlayer;
	bool m_isPause = false;
	std::string m_pseudo;
	
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
	UIManager* m_pUIManager;
};

// =======STATE-MACHINE======= //
/*struct StateShipGlobal
{
	void OnEnter(Ship& cur, int from);
	void OnExecute(Ship& cur);
	void OnExit(Ship& cur, int to);
};

struct StateShipIdle
{
	void OnEnter(Ship& cur, int from);
	void OnExecute(Ship& cur);
	void OnExit(Ship& cur, int to);
};

struct StateShipBlink
{
	void OnEnter(Ship& cur, int from);
	void OnExecute(Ship& cur);
	void OnExit(Ship& cur, int to);
};*/
