#pragma once

class BlendState
{
public:
    BlendState();
    ~BlendState();

    void SetState();

    void Alpha(bool value);
    void Additive();
    void AlphaToCoverage(bool value);

    void Changed();

private:
    D3D11_BLEND_DESC m_desc = {};
    ID3D11BlendState* m_state = nullptr;
};