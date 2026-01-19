#pragma once
#include "pch.h"

class InputText
{
public:
    InputText(cpu_font& _font, XMINT2 _pos) : m_font(&_font), m_pos(_pos) {}

    void Update();
    void Render();
    bool IsFinished() const { return m_finished;};
    const std::string& GetText() const { return m_text;};

private:
    XMINT2 m_pos;
    cpu_font* m_font;
    std::string m_text;
    bool m_finished = false;
};


