#include "UIManager.h"

inline UIManager* UIManager::s_pInstance = nullptr;

UIManager::UIManager()
{
    s_pInstance = this;
}

UIManager* UIManager::Get()
{
    if (s_pInstance == nullptr) s_pInstance = new UIManager();
    return s_pInstance;
}

UIManager::~UIManager()
{
    for (UI* ui : m_UIs)
        delete ui;

    m_UIs.clear();
    s_pInstance = nullptr;
}

void UIManager::Update()
{
    for (UI* ui : m_UIs)
        if (ui->IsActive())
            ui->Update();
}

void UIManager::Render()
{
    for (UI* ui : m_UIs)
        if (ui->IsActive())
            ui->Render();
}
