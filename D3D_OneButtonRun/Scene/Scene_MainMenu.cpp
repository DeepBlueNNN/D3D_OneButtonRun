#include "framework.h"
#include "Scene_MainMenu.h"

Scene_MainMenu::Scene_MainMenu()
{
	m_sceneName = "MainMenu";

	CAMERA->InitCamera();

	UITexture* texture = new UITexture(L"Textures/UI/Title_B.png", Vector2(1200.0f, 150.0f));
	texture->SetAlpha(1.0f);
	m_textures.push_back(texture);

	Button* button_Start = new Button(L"Textures/UI/Button_GameStart_N.png");
	button_Start->SetTag("Start");
	button_Start->SetHoverEffct(L"Textures/UI/Button_GameStart_Y.png", 1.0f);
	button_Start->SetAction(Button::CHANGESCENE, "SelectStage");
	m_buttons.push_back(button_Start);

	Button* button_MapEditor = new Button(L"Textures/UI/Button_MapEditor_N.png");
	button_MapEditor->SetTag("MapEditor");
	button_MapEditor->SetHoverEffct(L"Textures/UI/Button_MapEditor_Y.png", 1.0f);
	button_MapEditor->SetAction(Button::CHANGESCENE, "MapEditor");
	m_buttons.push_back(button_MapEditor);

	Button* button_Quit = new Button(L"Textures/UI/Button_Quit_N.png");
	button_Quit->SetTag("Quit");
	button_Quit->SetHoverEffct(L"Textures/UI/Button_Quit_Y.png", 1.0f);
	button_Quit->SetAction(Button::QUIT);
	m_buttons.push_back(button_Quit);
}

Scene_MainMenu::~Scene_MainMenu()
{
	for (Button* button : m_buttons)
		SAFE_DELETE(button);
	m_buttons.erase(m_buttons.begin(), m_buttons.end());

	for (UITexture* texture : m_textures)
		SAFE_DELETE(texture);
	m_textures.erase(m_textures.begin(), m_textures.end());
}

void Scene_MainMenu::Update()
{
	for (int i = 0; i < m_textures.size(); ++i)
	{
		m_textures[i]->Pos() = Vector3(MAIN->GetWidth() / 2.0f, (MAIN->GetHeight() / 2.0f) + 100.0f, 1.0f);
	}

	for (int i = 0; i < m_buttons.size(); ++i)
	{
		m_buttons[i]->Pos() = Vector3(MAIN->GetWidth() / 2.0f, (MAIN->GetHeight() / 2.0f) - (i * 100.0f + 100.0f), 1.0f);

		if (m_buttons[i]->CheckMouseCollision(mousePos))
		{
			m_buttons[i]->OnHover(true);

			if (KEY_DOWN(VK_LBUTTON))
			{
				m_buttons[i]->OnClicked();
			}
			continue;
		}

		m_buttons[i]->OnHover(false);
	}
}

void Scene_MainMenu::Render()
{
}

void Scene_MainMenu::PreRender()
{
}

void Scene_MainMenu::PostRender()
{
	ENV->PostSet();

	for (UITexture* texture : m_textures)
		texture->Render();

	for (Button* button : m_buttons)
		button->Render();

	ENV->Set();
}

void Scene_MainMenu::GUIRender()
{
	for(Button* button : m_buttons)
		button->GUIRender();
}
