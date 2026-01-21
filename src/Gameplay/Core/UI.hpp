#pragma once
#include "pch.h"

class UI
{
public:
    UI() = default;
    virtual ~UI() = default;
    virtual void Update() =0;
    virtual void Render() = 0;
    
    bool IsActive() { return m_isActive; }
    void SetActive(bool _state) { m_isActive = _state; }

protected:
    bool m_isActive = false;
    
    XMFLOAT2 m_pos;
    XMFLOAT2 m_size;
    
private:
};
