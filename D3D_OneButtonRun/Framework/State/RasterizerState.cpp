#include "Framework.h"

RasterizerState::RasterizerState()
{
    m_desc.CullMode = D3D11_CULL_BACK;
    m_desc.FillMode = D3D11_FILL_SOLID;

    Changed();
}

RasterizerState::~RasterizerState()
{
    m_state->Release();
}

void RasterizerState::SetState()
{
    DC->RSSetState(m_state);
}

void RasterizerState::FillMode(D3D11_FILL_MODE value)
{
    m_desc.FillMode = value;

    Changed();
}

void RasterizerState::FrontCounterClockwise(bool value)
{
    m_desc.FrontCounterClockwise = value;

    Changed();
}

void RasterizerState::Changed()
{
    if (m_state != nullptr)
        m_state->Release();

    DEVICE->CreateRasterizerState(&m_desc, &m_state);
}
