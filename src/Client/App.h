#pragma once

#include "InputText.h"
#include "Gameplay/Core/GameManager.h"

class Player;
class GameManager;
class UIManager;

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
	GameManager m_pGameManager;

	float m_width = 25.0f;
	float m_height = 25.0f;
	
	// Resources
	cpu_font m_font;
	cpu_rt* m_rts[1];
	
	// UI
	UIManager* m_pUIManager;
};