#include "Framework.h"

Transform::Transform()
{
    m_world = XMMatrixIdentity();
}

void Transform::UpdateWorld()
{
	// 집벌락 방지하기 위해  FQUAT
    m_world = XMMatrixTransformation(m_pivot,
        XMQuaternionIdentity(), m_localScale, m_pivot,
        XMQuaternionRotationRollPitchYawFromVector(m_localRotation),
        m_localPosition);

    if (m_parent)
        m_world *= m_parent->m_world;

    XMStoreFloat4x4(&m_matWorld, m_world);
    m_right    = Vector3(m_matWorld._11, m_matWorld._12, m_matWorld._13);
    m_up       = Vector3(m_matWorld._21, m_matWorld._22, m_matWorld._23);
    m_forward  = Vector3(m_matWorld._31, m_matWorld._32, m_matWorld._33);

    XMVECTOR outS, outR, outT;
    XMMatrixDecompose(&outS, &outR, &outT, m_world);

    Float3 tempPos, tempScale;
    XMStoreFloat3(&tempPos, outT);
    XMStoreFloat3(&tempScale, outS);

    m_globalPosition = tempPos;
    m_globalScale = tempScale;    
}

void Transform::GUIRender()
{
    if (ImGui::TreeNode((m_tag + "_Transform").c_str()))
    {
        ImGui::Text(m_tag.c_str());
    
        ImGui::Checkbox("Active", &m_isActive);
    
        string temp = m_tag + "_Pos";
        ImGui::DragFloat3(temp.c_str(), (float*)&m_localPosition, 0.1f);
    
        temp = m_tag + "_Rot";
        Float3 rot;
        rot.x = XMConvertToDegrees(m_localRotation.x);
        rot.y = XMConvertToDegrees(m_localRotation.y);
        rot.z = XMConvertToDegrees(m_localRotation.z);
    
        ImGui::DragFloat3(temp.c_str(), (float*)&rot, 1.0f, -180, 180);
    
        m_localRotation.x = XMConvertToRadians(rot.x);
        m_localRotation.y = XMConvertToRadians(rot.y);
        m_localRotation.z = XMConvertToRadians(rot.z);
    
        temp = m_tag + "_Scale";
        ImGui::DragFloat3(temp.c_str(), (float*)&m_localScale, 0.1f);
    
        ImGui::TreePop();
    }    
}

Transform* Transform::GetParent()
{
    if (m_parent != nullptr)
        return m_parent->GetParent();

    return this;
}

bool Transform::Active()
{
    if (m_parent == nullptr)
        return m_isActive;

    if (m_isActive == false)
        return false;

    return m_parent->Active();
}