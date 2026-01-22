#include "UIDialogue.h"

#include "Client/App.h"

UIDialogue::UIDialogue()
{
    m_answer = nullptr;
    m_isActive = true;
    App::GetInstance().m_isPause = true;
}

void UIDialogue::Init(std::string& _answer)
{
    m_answer = &_answer;
}

void UIDialogue::AddInputText(cpu_font& _font, XMFLOAT2 _pos, int _anchor, std::string _text)
{
    InputText* newInputText = new InputText();
    newInputText->Init(_font, _pos, _anchor, _text);
    m_dialogue.push_back(newInputText);
}

void UIDialogue::Update()
{
    if (m_dialogue[m_currentIndex]->IsFinished())
    {
         *m_answer = m_dialogue[m_currentIndex]->GetText();
         if (m_dialogue.size() > m_currentIndex + 1)
         {
             m_currentIndex++;
         }
         else
         {
             App::GetInstance().m_isPause = false;
             m_isActive = false;
         }
    }
    m_dialogue[m_currentIndex]->Update();
}

void UIDialogue::Render()
{
    m_dialogue[m_currentIndex]->Render();
}
