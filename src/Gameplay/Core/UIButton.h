#pragma once
#include "pch.h"
#include "UI.hpp"
#include <functional>

class UIButton : public UI
{
public:
    UIButton();
    ~UIButton() override ;
    void Init(XMINT2 _pos, XMINT2 _size, std::string const& _path, std::string const& _hover);
    void Update() override ;
    void Render() override { };
    void SetVisible(bool _state) override { m_pSprite->visible = _state; m_isActive = _state; };
    void SetOnClickCallback(std::function<void()> _callback);
protected:
private:
    cpu_sprite* m_pSprite = nullptr;

    cpu_texture m_texture;
    cpu_texture m_textureHover;

    std::function<void()> m_onClickCallback;
};
