#include "Framework.h"

BlendState::BlendState()
{    
    m_desc.RenderTarget[0].BlendEnable = false;
    m_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    m_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    m_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    
    m_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    m_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    m_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    
    m_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;        

    Changed();
}

BlendState::~BlendState()
{
    m_state->Release();
}

void BlendState::SetState()
{
    float blendFactor[4] = {};
    DC->OMSetBlendState(m_state, blendFactor, 0xffffffff);
}

void BlendState::Alpha(bool value)
{
    m_desc.RenderTarget[0].BlendEnable = value;
    m_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    m_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    m_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    Changed();
}

void BlendState::Additive()
{
    m_desc.RenderTarget[0].BlendEnable = true;
    m_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    m_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    m_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    Changed();
}

void BlendState::AlphaToCoverage(bool value)
{
    m_desc.AlphaToCoverageEnable = value;

    Changed();
}

void BlendState::Changed()
{
    if (m_state != nullptr)
        m_state->Release();

    DEVICE->CreateBlendState(&m_desc, &m_state);
}
