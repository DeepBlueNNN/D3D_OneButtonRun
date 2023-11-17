#include "Framework.h"

Camera::Camera()
{
    m_tag = "Camera";

    m_viewBuffer = new ViewBuffer();
    m_viewBuffer->SetVS(1);

    // Init Camera Transfrom
    Pos().x = 0.0;
    Pos().y = 4.7f;
    Pos().z = -20.3f;

    Rot().x = 0.0f;
    Rot().y = 0.0f;
    Rot().z = 0.0f;

    Scale().x = 1.0f;
    Scale().y = 1.0f;
    Scale().z = 1.0f;
}

Camera::~Camera()
{
    SAFE_DELETE(m_viewBuffer);

    Save();
}

void Camera::Update()
{
    m_projection = ENV->GetProjection();

    FreeMode();

    SetView();
}

void Camera::GUIRender()
{
    if (ImGui::TreeNode("CameraOption"))
    {
        ImGui::DragFloat("MoveSpeed", &m_moveSpeed);
        ImGui::DragFloat("RotSpeed", &m_rotSpeed);

        Transform::GUIRender();

        ImGui::TreePop();
    }
}

void Camera::SetView()
{
    UpdateWorld();

    m_view = XMMatrixInverse(nullptr, m_world);
    m_viewBuffer->Set(m_view, m_world);
    m_viewBuffer->SetVS(1);
}

Vector3 Camera::ScreenToWorld(Vector3 pos)
{
    return XMVector3TransformCoord(pos, m_world);
}

Vector3 Camera::WorldToScreen(Vector3 worldPos)
{
    Vector3 screenPos;

    screenPos = XMVector3TransformCoord(worldPos, m_view);
    screenPos = XMVector3TransformCoord(screenPos, m_projection);
    //NDC : -1 ~ 1

    screenPos = (screenPos + Vector3::One()) * 0.5f;//0~1

    screenPos.x *= MAIN->GetWidth();
    screenPos.y *= MAIN->GetHeight();

    return screenPos;
}

Ray Camera::ScreenPointToRay(Vector3 screenPoint)
{
    Vector3 screenSize((float)MAIN->GetWidth(), (float)MAIN->GetHeight(), 1.0f);
    Vector2 point;

    point.x = (screenPoint.x / screenSize.x) * 2.0f - 1.0f;   // -1.0f ~ 1.0f;
    point.y = (screenPoint.y / screenSize.y) * 2.0f - 1.0f;   // -1.0f ~ 1.0f;

    Matrix projection = ENV->GetProjection();

    MatrixF temp;
    XMStoreFloat4x4(&temp, projection);   // 11(Sx),22(Sy)

    screenPoint.x = point.x / temp._11;
    screenPoint.y = point.y / temp._22;
    screenPoint.z = 1.0f;

    screenPoint = XMVector3TransformNormal(screenPoint, m_world);

    // Ray의 position과 direction 반환
    Ray ray;
    ray.pos = Pos();
    ray.dir = screenPoint.GetNormalized();

    return ray;
}

void Camera::FreeMode()
{
    if(GetAsyncKeyState(VK_RBUTTON))
    {
        if (KEY_PRESS('W'))
            Pos() += Forward()  * m_moveSpeed * DELTA;
        if (KEY_PRESS('S'))
            Pos() -= Forward()  * m_moveSpeed * DELTA;
        if (KEY_PRESS('A'))
            Pos() += Left()     * m_moveSpeed * DELTA;
        if (KEY_PRESS('D'))
            Pos() += Right()    * m_moveSpeed * DELTA;
        if (KEY_PRESS('E'))
            Pos() += Down()     * m_moveSpeed * DELTA;
        if (KEY_PRESS('Q'))
            Pos() += Up()       * m_moveSpeed * DELTA;

        ImVec2 delta = ImGui::GetIO().MouseDelta;

        Rot().x += delta.y * m_rotSpeed * DELTA;
        Rot().y += delta.x * m_rotSpeed * DELTA;
    }
}