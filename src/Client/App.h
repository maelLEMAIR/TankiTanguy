#pragma once

#include "Gameplay/Core/GameManager.h"
#include "Network/ClientSocket.h"

class Player;
class GameManager;
class UIManager;
class UIDialogue;
class UIText;

class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void OnStart();
	void OnUpdate();
	void SetUI();
	void OnExit();
	void OnRender(int pass);

	void Log();
	void SetLog(bool _state) { m_isLogin = _state; }
	
	std::string m_addr;
	std::string m_port;
	std::string m_pseudo;
	
private:
	inline static App* s_pApp = nullptr;
	GameManager m_gameManager;

	UIDialogue* m_dialogueLog = nullptr;
	UIText* m_uiScore = nullptr;
	
	ClientSocket m_client;
	float m_requestTime = 3.0f;
	float m_requestProgress = 0.0f;

	bool m_isLogin = false;
	bool m_isRequested = false;
	bool m_isRunning = false;

	float m_width = 50.0f;
	float m_height = 50.0f;
	
	// Resources
	cpu_font m_font;
	cpu_rt* m_rts[1];

	// Player
	int m_id = -1;
	Player* m_pPlayer = nullptr;
	
	// UI
	UIManager* m_pUIManager;

	// Map
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
