#pragma once
#include "InputText.h"

class App;

class UIDialogue : public UI
{
public:
    UIDialogue();
    void Init(std::string& _answer);
    ~UIDialogue() override { m_dialogue.clear(); };
    void AddInputText(cpu_font& _font, XMINT2 _pos, int _anchor, std::string _text);
    void Update() override;
    void Render() override;
    void SetVisible(bool _state) override { };
protected:

private:
    int m_currentIndex = 0;
    std::string* m_answer;
    std::vector<InputText*> m_dialogue;
};