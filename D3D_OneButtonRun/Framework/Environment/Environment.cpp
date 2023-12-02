#include "Framework.h"
#include "Environment.h"

Environment::Environment()
{
    CreateProjection();
    CreateState();

    m_viewBuffer  = new ViewBuffer();    
}

Environment::~Environment()
{
    SAFE_DELETE(m_projectionBuffer);  
    SAFE_DELETE(m_viewBuffer);
    SAFE_DELETE(m_samplerState);
    SAFE_DELETE(m_rasterizerState[0]);
    SAFE_DELETE(m_rasterizerState[1]);
    SAFE_DELETE(m_blendState[0]);
    SAFE_DELETE(m_blendState[1]);
}

void Environment::Update()
{
    if (KEY_DOWN(VK_F1))
        m_isWireMode = !m_isWireMode;

    if (KEY_DOWN(VK_F2))
        Collider::RenderOnOff();

    CAMERA->Update();
}

void Environment::GUIRender()
{
    if (ImGui::TreeNode(u8"환경 설정"))
    {
        // Camera
        CAMERA->GUIRender();

        // Light
        if (ImGui::TreeNode(u8"라이트 설정"))
        {
            if (ImGui::TreeNode(u8"라이트"))
            {
                ImGui::ColorEdit3("Color", (float*)&SAVELOAD->GetLightBuffer()->Get().lights[0].color, ImGuiColorEditFlags_PickerHueWheel);
                ImGui::SliderFloat3("Dir", (float*)&SAVELOAD->GetLightBuffer()->Get().lights[0].direction, -1, 1);

                ImGui::TreePop();
            }

            if (ImGui::TreeNode(u8"엠비언트"))
            {
                ImGui::ColorEdit3("Light", (float*)&SAVELOAD->GetLightBuffer()->Get().ambientLight, ImGuiColorEditFlags_PickerHueWheel);
                ImGui::ColorEdit3("Ceil", (float*)&SAVELOAD->GetLightBuffer()->Get().ambientCeil, ImGuiColorEditFlags_PickerHueWheel);

                ImGui::TreePop();
            }

            bool ret;

            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.8f, 0.8f));
            ret = ImGui::Button(u8"라이트 초기화", ImVec2(100, 20));
            ImGui::PopStyleColor(3);

            if (ret)
            {
                SAVELOAD->GetLightBuffer()->Init();
            }

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}

void Environment::Set()
{
    SetViewport();
    SetPerspective();

    if (m_isWireMode)
        m_rasterizerState[1]->SetState();
    else
        m_rasterizerState[0]->SetState();

    m_blendState[0]->SetState();
    m_depthStencilState[0]->SetState();

    SAVELOAD->GetLightBuffer()->SetPS(0);
}

void Environment::PostSet()
{
    m_viewBuffer->SetVS(1);
    SetOrthographic();

    m_blendState[1]->SetState();
    m_depthStencilState[1]->SetState();
}

void Environment::SetViewport(UINT width, UINT height)
{
    m_viewport.Width = width;
    m_viewport.Height = height;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;

    DC->RSSetViewports(1, &m_viewport);
}

void Environment::SetPerspective()
{
    m_projectionBuffer->Set(m_perspective);
    m_projectionBuffer->SetVS(2);
}

void Environment::SetOrthographic()
{
    m_projectionBuffer->Set(m_orthographic);
    m_projectionBuffer->SetVS(2);
}

void Environment::UpdateProjection()
{
    m_orthographic = XMMatrixOrthographicOffCenterLH(
        0.0f, MAIN->GetWidth(), 0.0f, MAIN->GetHeight(), -1.0f, 1.0f);

    m_perspective = XMMatrixPerspectiveFovLH(XM_PIDIV4,
        (float)MAIN->GetWidth() / (float)MAIN->GetHeight(), 0.1f, 1000.0f);

    SetPerspective();
}

void Environment::CreateProjection()
{
    m_orthographic = XMMatrixOrthographicOffCenterLH(
        0.0f, MAIN->GetWidth(), 0.0f, MAIN->GetHeight(), -1.0f, 1.0f);
    
    m_perspective = XMMatrixPerspectiveFovLH(XM_PIDIV4,
        (float)MAIN->GetWidth() / (float)MAIN->GetHeight(), 0.1f, 1000.0f);

    m_projectionBuffer = new MatrixBuffer();    
}

void Environment::CreateState()
{
    m_samplerState = new SamplerState();
    m_samplerState->SetState();

    m_rasterizerState[0] = new RasterizerState();
    m_rasterizerState[1] = new RasterizerState();
    m_rasterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);

    m_blendState[0] = new BlendState();
    m_blendState[1] = new BlendState();
    m_blendState[1]->Alpha(true);

    m_depthStencilState[0] = new DepthStencilState();
    m_depthStencilState[1] = new DepthStencilState();
    m_depthStencilState[1]->DepthEnable(false);
}
