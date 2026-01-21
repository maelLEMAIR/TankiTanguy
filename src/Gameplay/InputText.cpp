#include "InputText.h"

void InputText::Update()
{
    if (m_finished)
        return;

    // Lettres A-Z
    for (char c = 'A'; c <= 'Z'; c++)
        if (cpuInput.IsKeyDown(c))
            m_text += c;

    // Chiffres
    for (char c = '0'; c <= '9'; c++)
        if (cpuInput.IsKeyDown(c))
            m_text += c;
    
    int temp = 0;
    for (int i = VK_NUMPAD0; i <= VK_NUMPAD9; i++)
    {
        if (cpuInput.IsKeyDown(i))
            m_text += std::to_string(temp);
        temp++;
    }
    
    if (cpuInput.IsKeyDown(VK_OEM_PERIOD) || cpuInput.IsKeyDown(VK_DECIMAL))
        m_text += '.';

    // Backspace
    if (cpuInput.IsKeyDown(VK_BACK))
        if (!m_text.empty())
            m_text.pop_back();
    
    // Validation
    if (cpuInput.IsKeyDown(VK_RETURN))
        m_finished = true;
}

void InputText::Render()
{
    cpuDevice.DrawText(m_font,
        m_text.c_str(),
        m_pos.x, m_pos.y,
        m_anchor);
}
