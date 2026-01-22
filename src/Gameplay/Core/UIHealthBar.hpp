#pragma once
#include "pch.h"
#include "Player.hpp"
#include "UI.hpp"

class UIHealthBar : public UI
{
public:
    void Init(cpu_font& _font, XMFLOAT2 _pos, float _size, Player& _player, std::string const& _path);
    void Update() override;
    void Render() override;

private:
    std::vector<cpu_sprite*> m_sprites;
    cpu_texture m_text;
    cpu_font m_font;
    Player* m_pPlayer = nullptr;
};

inline void UIHealthBar::Init(cpu_font& _font, XMFLOAT2 _pos, float _size, Player& _player, std::string const& _path)
{
    m_pos = _pos;
    m_font = _font;
    m_text =  cpu_texture();
    m_text.Load(_path.c_str());
    for (int i = 0; i < 5; i++)
    {
        cpu_sprite* sprite = cpuEngine.CreateSprite();
        sprite->pTexture = &m_text;
        sprite->x = static_cast<int>(_pos.x) + i * static_cast<int>(_size);
        sprite->y = static_cast<int>(_pos.y);
        m_sprites.push_back(sprite);
    }
    m_pPlayer = &_player;
}

inline void UIHealthBar::Update()
{
    int currentHP = m_pPlayer->GetLife().GetCurrentLife();
    int maxHP = m_pPlayer->GetLife().GetMaxLife();

    int hpPerSprite = std::max(1, maxHP / static_cast<int>(m_sprites.size()));

    for (int i = 0; i < m_sprites.size(); i++)
    {
        m_sprites[i]->visible = (currentHP > i * hpPerSprite);
    }
}

inline void UIHealthBar::Render()
{
    std::string text = std::to_string(m_pPlayer->GetLife().GetCurrentLife());
    cpuDevice.DrawText(&m_font, text.c_str(), m_pos.x, m_pos.y + 6, CPU_TEXT_LEFT);
}
