#pragma once

#include "InputText.h"
#include "Gameplay/Player.hpp"
#include "Gameplay/PlayerController.hpp"

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

private:
	inline static App* s_pApp = nullptr;

	// Resources
	cpu_font m_font;
	cpu_rt* m_rts[1];

	// 3D
	Player* m_pPlayer;
	PlayerController* m_pPlCrt;
	Player* m_pPlayer2;
	Player* m_pPlayer3;

	// UI
	InputText* m_pseudo;
	InputText* m_loginAdrr;
	InputText* m_loginPort;
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
