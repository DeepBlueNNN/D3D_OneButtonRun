#pragma once

/////////////////////////////////////////////////////////////////
//                   Projection 생성 Class				       //
//              Rasterizer,Blend,Depth등을 Control             //
////////////////////////////////////////////////////////////////

class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

private:
    Environment();
    ~Environment();

public:
    void Update();
    void GUIRender();

public: // Getter
    Matrix  GetProjection() { return m_perspective; }

public: // Setter
    void Set();
    void PostSet();

    void SetViewport(UINT width = MAIN->GetWidth(), UINT height = MAIN->GetHeight());
    void SetPerspective();
    void SetOrthographic();
    void SetRenderMode(bool isRender);

public:
    void UpdateProjection();

private:
    void CreateProjection();
    void CreateState();

private:
    MatrixBuffer*       m_projectionBuffer     = nullptr;       
    ViewBuffer*         m_viewBuffer           = nullptr;
    
    SamplerState*       m_samplerState         = nullptr;
    RasterizerState*    m_rasterizerState[2];
    BlendState*         m_blendState[2];
    DepthStencilState*  m_depthStencilState[2];

    D3D11_VIEWPORT      m_viewport;

    Matrix  m_perspective;
    Matrix  m_orthographic;
    bool    m_isWireMode = false;
    bool    m_isRenderMode = true;
};