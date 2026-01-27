#pragma once
#include "pch.h"
#include "UI.hpp"

class UIText : public UI
{
    public:
    UIText() { };
    ~UIText()       override { };
    void Update()   override { };
    void Render()   override;

    void Init(cpu_font& _font, XMINT2 _pos, int _anchor, const std::string& _text);
    void Init(cpu_font& _font, XMINT2 _pos, int _anchor, int _value);
    void SetText(const std::string& _text);
    void SetText(int _value);

private:
    std::string m_text;
    
    int m_intText       = -1;
    cpu_font* m_font    = nullptr;
    int m_anchor        = 0;
};

inline void UIText::Render()
{
    cpuDevice.DrawText(m_font, m_text.c_str(), m_pos.x, m_pos.y, m_anchor);
}

inline void UIText::Init(cpu_font& _font, XMINT2 _pos, int _anchor, const std::string& _text)
{
    m_font = &_font;
    m_pos = _pos;
    m_anchor = _anchor;
    m_text = _text;
}

inline void UIText::Init(cpu_font& _font, XMINT2 _pos, int _anchor, int _value)
{
    m_font = &_font;
    m_pos = _pos;
    m_anchor = _anchor;
    m_text = std::to_string(_value);
}

inline void UIText::SetText(const std::string& _text)
{
    m_text = _text;
}

inline void UIText::SetText(int _value)
{
    m_text = std::to_string(_value);
}
