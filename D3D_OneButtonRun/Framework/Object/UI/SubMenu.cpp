#include "framework.h"
#include "SubMenu.h"

SubMenu::SubMenu()
{
	UITexture* texture = new UITexture(L"Textures/Color/Black.png", Vector2(MAIN->GetWidth(), MAIN->GetHeight()));
	texture->SetAlpha(0.5f);
	m_textures.push_back(texture);

	texture = new UITexture(L"Textures/UI/Title_W.png", Vector2(600.0f, 100.0f));
	m_textures.push_back(texture);

	Button* button = new Button(L"Textures/UI/Button_SelectStage_N.png", Vector2(300.0f, 50.0f));
	button->SetTag("SelectStage");
	button->SetHoverEffct(L"Textures/UI/Button_SelectStage_Y.png", 1.0f);
	button->SetAction(Button::CHANGESCENE, "SelectStage");
	m_buttons.push_back(button);

	button = new Button(L"Textures/UI/Button_MainMenu_N.png", Vector2(300.0f, 50.0f));
	button->SetTag("MainMenu");
	button->SetHoverEffct(L"Textures/UI/Button_MainMenu_Y.png", 1.0f);
	button->SetAction(Button::CHANGESCENE, "MainMenu");
	m_buttons.push_back(button);

	button = new Button(L"Textures/UI/Button_QuitGame_N.png", Vector2(300.0f, 50.0f));
	button->SetTag("Quit");
	button->SetHoverEffct(L"Textures/UI/Button_QuitGame_Y.png", 1.0f);
	button->SetAction(Button::QUIT);
	m_buttons.push_back(button);

	button = new Button(L"Textures/UI/Button_Return_N.png", Vector2(300.0f, 50.0f));
	button->SetTag("Quit");
	button->SetHoverEffct(L"Textures/UI/Button_Return_Y.png", 1.0f);
	button->SetAction(Button::MAX);
	m_buttons.push_back(button);
}

SubMenu::~SubMenu()
{
	for (Button* button : m_buttons)
		SAFE_DELETE(button);
	m_buttons.erase(m_buttons.begin(), m_buttons.end());

	for (UITexture* texture : m_textures)
		SAFE_DELETE(texture);
	m_textures.erase(m_textures.begin(), m_textures.end());
}

void SubMenu::Update()
{
	// 패키징 시 ESC키로 변환 <- 변환완료
//	if (KEYBOARD->Down(VK_TAB))
	if (KEYBOARD->Down(VK_ESCAPE))
	{
		if (m_isPanelOn)
			m_isPanelOn = false;
		else
			m_isPanelOn = true;
	}

	if (m_isPanelOn)
	{
		m_textures[0]->Pos() = Vector3(MAIN->GetWidth() / 2.0f, (MAIN->GetHeight() / 2.0f), 1.0f);
		m_textures[1]->Pos() = Vector3(MAIN->GetWidth() / 3.0f, (MAIN->GetHeight()) - 200.0f, 1.0f);

		for (int i = 0; i < m_buttons.size(); ++i)
		{
			m_buttons[i]->Pos() = Vector3(MAIN->GetWidth() / 3.0f, (MAIN->GetHeight()) - (i * 100.0f + 400.0f), 1.0f);

			if (m_buttons[i]->CheckMouseCollision(mousePos))
			{
				m_buttons[i]->OnHover(true);

				if (KEY_DOWN(VK_LBUTTON))
				{
					m_buttons[i]->OnClicked();
					m_isPanelOn = false;
				}
				continue;
			}

			m_buttons[i]->OnHover(false);
		}
	}
}

void SubMenu::Render()
{
	if (m_isPanelOn)
	{
		ENV->PostSet();

		for (UITexture* texture : m_textures)
			texture->Render();

		for (Button* button : m_buttons)
			button->Render();

		ENV->Set();
	}
}