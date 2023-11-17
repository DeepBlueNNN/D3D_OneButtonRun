#include "Framework.h"

SamplerState::SamplerState()
{
    m_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    m_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    m_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    m_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;;
    m_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    m_desc.MinLOD = 0;
    m_desc.MaxLOD = D3D11_FLOAT32_MAX;
    //LOD(Level Of Detail) : 카메라의 거리에 따라서 퀄리티를 다르게 하는 것   

    Changed();
}

SamplerState::~SamplerState()
{
    m_state->Release();
}

void SamplerState::SetState(UINT slot)
{
    DC->PSSetSamplers(slot, 1, &m_state);
}

void SamplerState::Filter(D3D11_FILTER value)
{
    m_desc.Filter = value;

    Changed();
}

void SamplerState::Changed()
{
    if (m_state != nullptr)
        m_state->Release();

    DEVICE->CreateSamplerState(&m_desc, &m_state);
}
