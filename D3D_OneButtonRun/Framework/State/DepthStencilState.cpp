#include "Framework.h"

DepthStencilState::DepthStencilState()
{
    m_desc.DepthEnable = true;
    m_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    m_desc.DepthFunc = D3D11_COMPARISON_LESS;

    m_desc.StencilEnable = true;
    m_desc.StencilReadMask = 0xff;
    m_desc.StencilWriteMask = 0xff;

    m_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    m_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    m_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    m_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    m_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    m_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    m_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    m_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    Changed();
}

DepthStencilState::~DepthStencilState()
{
    m_state->Release();
}

void DepthStencilState::SetState()
{
    DC->OMSetDepthStencilState(m_state, 1);
}

void DepthStencilState::DepthEnable(bool value)
{
    m_desc.DepthEnable = value;

    Changed();
}

void DepthStencilState::DepthWriteMask(D3D11_DEPTH_WRITE_MASK value)
{
    m_desc.DepthWriteMask = value;

    Changed();
}

void DepthStencilState::Changed()
{
    if (m_state != nullptr)
        m_state->Release();

    DEVICE->CreateDepthStencilState(&m_desc, &m_state);
}
