#include "framework.h"
#include "Sky.h"

Sky::Sky(wstring key)
{
	m_sphere = new Sphere();
	m_sphere->GetMaterial()->SetShader(L"LandScape/Sky.hlsl");

	StringPath::GetFiles(m_skyTextureFiles, L"Textures/CubeMap/", L"*.dds", false);
	for (int i = 0; i < m_skyTextureFiles.size(); ++i)
	{
		m_cubeTexture->Add(m_skyTextureFiles[i]);
		m_comboList.push_back(StringPath::GetFileNameWithoutExtension(StringPath::ToString(m_skyTextureFiles[i])));
	}

	m_cubeTexture = m_cubeTexture->GetTexture(key);

	m_rasterizerState[0] = new RasterizerState();
	m_rasterizerState[1] = new RasterizerState();
	m_rasterizerState[1]->FrontCounterClockwise(true);

	m_depthStencilState[0] = new DepthStencilState();
	m_depthStencilState[1] = new DepthStencilState();
	m_depthStencilState[1]->DepthEnable(false);
}

Sky::~Sky()
{
	SAFE_DELETE(m_sphere);
	SAFE_DELETE(m_rasterizerState[0]);
	SAFE_DELETE(m_rasterizerState[1]);
	SAFE_DELETE(m_depthStencilState[0]);
	SAFE_DELETE(m_depthStencilState[1]);
}

void Sky::Render()
{
	m_cubeTexture->PSSet(10);
	m_rasterizerState[1]->SetState();
	m_depthStencilState[1]->SetState();

	m_sphere->Render();
	m_rasterizerState[0]->SetState();
	m_depthStencilState[0]->SetState();
}

void Sky::GUIRender()
{
	if (ImGui::TreeNode(u8"Å¥ºê¸Ê"))
	{
		string current_item;

		if (ImGui::BeginCombo("##combo", current_item.c_str()))
		{
			for (int n = 0; n < m_skyTextureFiles.size(); n++)
			{
				bool is_selected = (current_item == m_comboList[n]);
				if (ImGui::Selectable(m_comboList[n].c_str(), is_selected))
				{
					current_item = m_comboList[n];
					is_selected = true;
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
					m_cubeTexture = m_cubeTexture->GetTexture(m_skyTextureFiles[n]);
				}
			}
			ImGui::EndCombo();
		}

		ImGui::TreePop();
	}
}

void Sky::SetTexture(wstring textureFile)
{
	m_cubeTexture = m_cubeTexture->GetTexture(textureFile);
}
