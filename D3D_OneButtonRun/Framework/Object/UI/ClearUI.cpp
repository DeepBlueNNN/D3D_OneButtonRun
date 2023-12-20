#include "framework.h"
#include "ClearUI.h"

ClearUI::ClearUI()
{
	UITexture* texture = new UITexture(L"Textures/Color/White.png", Vector2(MAIN->GetWidth(), MAIN->GetHeight()));
	texture->SetAlpha(0.1f);
	m_textures.push_back(texture);

	texture = new UITexture(L"Textures/UI/Texture_ClearReturn.png", Vector2(1000.0f, 100.0f));
	m_textures.push_back(texture);
}

ClearUI::~ClearUI()
{
	for (UITexture* texture : m_textures)
		SAFE_DELETE(texture);
	m_textures.erase(m_textures.begin(), m_textures.end());
}

void ClearUI::Update()
{
	if (m_isPanelOn)
	{
		m_textures[0]->Pos() = Vector3(MAIN->GetWidth() / 2.0f, MAIN->GetHeight() / 2.0f);
		m_textures[1]->Pos() = Vector3(MAIN->GetWidth() / 2.0f, MAIN->GetHeight() / 2.5f);
	}
}

void ClearUI::Render()
{
	if (m_isPanelOn)
	{
		ENV->PostSet();

		for (UITexture* texture : m_textures)
			texture->Render();

		ENV->Set();
	}
}
