#include "framework.h"
#include "HowToPlay.h"

HowToPlay::HowToPlay()
{
	UITexture* texture = new UITexture(L"Textures/Color/White.png", Vector2(MAIN->GetWidth(), MAIN->GetHeight()));
	texture->SetAlpha(0.5f);
	m_textures.push_back(texture);

	texture = new UITexture(L"Textures/UI/SPACE.png", Vector2(300.0f, 100.0f));
	m_textures.push_back(texture);

	texture = new UITexture(L"Textures/UI/Texture_InitPlace.png", Vector2(300.0f, 80.0f));
	m_textures.push_back(texture);

	texture = new UITexture(L"Textures/UI/ESC.png", Vector2(200.0f, 100.0f));
	m_textures.push_back(texture);

	texture = new UITexture(L"Textures/UI/Texture_SubMenu.png", Vector2(300.0f, 80.0f));
	m_textures.push_back(texture);
}

HowToPlay::~HowToPlay()
{
	for (UITexture* texture : m_textures)
		SAFE_DELETE(texture);
	m_textures.erase(m_textures.begin(), m_textures.end());
}

void HowToPlay::Update()
{
	if (m_isPanelOn)
	{
		m_textures[0]->Pos() = Vector3(MAIN->GetWidth() / 2.0f, MAIN->GetHeight() / 2.0f);
		m_textures[1]->Pos() = Vector3(MAIN->GetWidth() / 5.0f, ((MAIN->GetHeight() / 2.0f) - 150.0f), 1.0f);
		m_textures[2]->Pos() = Vector3((MAIN->GetWidth() / 5.0f) + 350.0f, ((MAIN->GetHeight() / 2.0f) - 150.0f), 1.0f);

		m_textures[3]->Pos() = Vector3(MAIN->GetWidth() / 5.0f, ((MAIN->GetHeight() / 2.0f) - 300.0f), 1.0f);
		m_textures[4]->Pos() = Vector3((MAIN->GetWidth() / 5.0f) + 350.0f, ((MAIN->GetHeight() / 2.0f) - 300.0f), 1.0f);
	}
}

void HowToPlay::Render()
{
	if (m_isPanelOn)
	{
		ENV->PostSet();

		for (UITexture* texture : m_textures)
			texture->Render();

		ENV->Set();
	}
}
