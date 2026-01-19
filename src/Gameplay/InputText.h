#pragma once
#include "pch.h"

class InputText
{
public:
    InputText(cpu_font& _font, XMINT2 _pos, int _anchor) : m_font(&_font), m_pos(_pos), m_anchor(_anchor) {}

    void Update();
    void Render();
    bool IsActive() const { return m_isActive;  }
    void SetActive(bool _state) { m_isActive = _state; }
    bool IsFinished() const { return m_finished;};
    const std::string& GetText() const { return m_text;};

private:
    int m_anchor = 0;
    bool m_isActive = false;
    XMINT2 m_pos;
    cpu_font* m_font;
    std::string m_text;
    bool m_finished = false;
};


