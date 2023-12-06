#include "framework.h"
#include "Scene_MainMenu.h"

Scene_MainMenu::Scene_MainMenu()
{
	m_sceneName = "MainMenu";

	Button* button_Start = new Button(L"Textures/UI/Button_GameStart_N.png");
	button_Start->SetHoverImage(L"Textures/UI/Button_GameStart_Y.png");
	button_Start->SetAction(Button::CHANGESCENE, "GamePlay");
	m_buttons.push_back(button_Start);

	Button* button_MapEditor = new Button(L"Textures/UI/Button_MapEditor_N.png");
	button_MapEditor->SetHoverImage(L"Textures/UI/Button_MapEditor_Y.png");
	button_MapEditor->SetAction(Button::CHANGESCENE, "MapEditor");
	m_buttons.push_back(button_MapEditor);

	Button* button_Quit = new Button(L"Textures/UI/Button_Quit_N.png");
	button_Quit->SetHoverImage(L"Textures/UI/Button_Quit_Y.png");
	button_Quit->SetAction(Button::QUIT);
	m_buttons.push_back(button_Quit);
}

Scene_MainMenu::~Scene_MainMenu()
{
	m_buttons.erase(m_buttons.begin(), m_buttons.end());
	m_buttons.clear();
}

void Scene_MainMenu::Update()
{
	for (int i =0; i<m_buttons.size(); ++i)
	{
		m_buttons[i]->Pos() = (Vector3(MAIN->GetWidth() / 2.0f, (MAIN->GetHeight() / 2.0f) - (i * 100.0f + 100.0f), 1.0f));

		if (m_buttons[i]->CheckMouseCollision(mousePos))
		{
			m_buttons[i]->OnHover(true);
			if (KEY_PRESS(VK_LBUTTON))
			{
				m_buttons[i]->OnClicked();
			}
			continue;
		}

		m_buttons[i]->OnHover(false);

		m_buttons[i]->Update();
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

	for (Button* button : m_buttons)
	{
		button->Render();
	}

	ENV->Set();

}

void Scene_MainMenu::GUIRender()
{
	for(Button* button : m_buttons)
		button->GUIRender();
}
