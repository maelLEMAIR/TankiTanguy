#include "UIButton.h"

UIButton::UIButton()
{
    
}

UIButton::~UIButton()
{
    cpuEngine.Release(m_pSprite);
}

void UIButton::Init(XMINT2 _pos, XMINT2 _size, std::string const& _path, std::string const& _hover)
{
    m_pos = _pos;
    m_size = _size;

    m_texture.Load(_path.c_str());
    m_textureHover.Load(_hover.c_str());
    
    m_pSprite = cpuEngine.CreateSprite();
    m_pSprite->pTexture = &m_texture;
    m_pSprite->x = _pos.x;
    m_pSprite->y = _pos.y;
}


void UIButton::Update()
{
    XMFLOAT2 ray;
    cpuEngine.GetCursor(ray);
    XMINT2 posCursor = XMINT2(static_cast<int>(ray.x), static_cast<int>(ray.y));

    if (posCursor.x >= m_pos.x && posCursor.x <= m_pos.x + m_size.x &&
        posCursor.y >= m_pos.y && posCursor.y <= m_pos.y + m_size.y)
    {
        m_pSprite->pTexture = &m_textureHover;
        if (cpuInput.IsKeyDown(VK_LBUTTON) && m_onClickCallback)
        {
            m_onClickCallback();
        }
    }
    else
    {
        m_pSprite->pTexture = &m_texture;
    }
}

void UIButton::SetOnClickCallback(std::function<void()> _callback)
{
    m_onClickCallback = _callback;
}


