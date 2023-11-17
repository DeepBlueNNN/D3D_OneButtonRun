#include    "framework.h"

GameObject::GameObject(wstring shaderFile)
{
	m_material   = new Material(shaderFile);
	m_worldBuffer = new MatrixBuffer();
}

GameObject::~GameObject()
{
	SAFE_DELETE(m_material);
	SAFE_DELETE(m_worldBuffer);
}

void GameObject::GUIRender()
{
	string label = m_tag + "_Object";
	m_material->SetName(m_tag);

	if (ImGui::TreeNode(label.c_str()))
	{
		Transform::GUIRender();
		m_material->GUIRender();

		ImGui::TreePop();
	}
}

void GameObject::SetRender()
{
	m_worldBuffer->Set(m_world);
	m_worldBuffer->SetVS(0);

	m_material->Set();
}