#pragma once

class DepthStencilState
{
public:
    DepthStencilState();
    ~DepthStencilState();

    void SetState();

    void DepthEnable(bool value);

    void DepthWriteMask(D3D11_DEPTH_WRITE_MASK value = D3D11_DEPTH_WRITE_MASK_ZERO);

    void Changed();

private:
    D3D11_DEPTH_STENCIL_DESC m_desc = {};
    ID3D11DepthStencilState* m_state = nullptr;
};