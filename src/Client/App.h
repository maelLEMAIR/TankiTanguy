#pragma once

#include "InputText.h"
#include "Core/UIManager.h"
#include "Gameplay/Core/GameManager.h"

class Player;

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
	
	bool m_isPause = false;
	std::string m_pseudo;
	
private:
	inline static App* s_pApp = nullptr;
	GameManager m_gameManager;

	float m_width = 50.0f;
	float m_height = 50.0f;
	
	// Resources
	cpu_font m_font;
	cpu_rt* m_rts[1];

	// Player
	Player* m_pPlayer = nullptr;
	
	// UI
	UIManager* m_pUIManager;

	// Map
	std::vector<cpu_entity*> m_floors;
	cpu_mesh m_pFloorMesh;
	cpu_material m_pFloorMaterial;
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
