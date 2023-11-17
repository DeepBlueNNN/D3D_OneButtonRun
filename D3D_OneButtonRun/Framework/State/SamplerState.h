#pragma once

class SamplerState
{
public:
    SamplerState();
    ~SamplerState();

    void SetState(UINT slot = 0);

    void Filter(D3D11_FILTER value);

    void Changed();

private:
    D3D11_SAMPLER_DESC m_desc = {};
    ID3D11SamplerState* m_state = nullptr;
};