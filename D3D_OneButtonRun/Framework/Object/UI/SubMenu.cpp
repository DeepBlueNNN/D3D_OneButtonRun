#include "framework.h"
#include "SubMenu.h"

SubMenu::SubMenu()
	: Quad(Vector2(MAIN->GetWidth(), MAIN->GetHeight()))
{
	m_material->SetShader(L"UI/Button.hlsl");
	m_backGround = Texture::Add(L"Textures/Color/Black.png");
	m_valueBuffer = new FloatValueBuffer();
	m_valueBuffer->Get()[0] = 0.2f;

	Button* button = new Button(L"Textures/UI/Button_SelectStage_N.png");
	button->SetTag("SelectStage");
	button->SetHoverEffct(L"Textures/UI/Button_SelectStage_Y.png", 1.0f);
	button->SetAction(Button::CHANGESCENE, "SelectStage");
	m_buttons.push_back(button);

	button = new Button(L"Textures/UI/Button_MainMenu_N.png");
	button->SetTag("MainMenu");
	button->SetHoverEffct(L"Textures/UI/Button_MainMenu_Y.png", 1.0f);
	button->SetAction(Button::CHANGESCENE, "MainMenu");
	m_buttons.push_back(button);

	button = new Button(L"Textures/UI/Button_Quit_N.png");
	button->SetTag("Quit");
	button->SetHoverEffct(L"Textures/UI/Button_Quit_Y.png", 1.0f);
	button->SetAction(Button::QUIT);
	m_buttons.push_back(button);

	button = new Button(L"Textures/UI/Button_Return_N.png");
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

	SAFE_DELETE(m_valueBuffer);
	SAFE_DELETE(m_backGround);
}

void SubMenu::Update()
{
	// 패키징 시 ESC키로 변환
	if (KEYBOARD->Down(VK_TAB))
	{
		if (m_isSubMenuOn)
			m_isSubMenuOn = false;
		else
			m_isSubMenuOn = true;
	}

	if (m_isSubMenuOn)
	{
		m_buttons[0]->Pos() = Vector3(0.0f, 0.0f, 1.0f);

		for (int i = 1; i < m_buttons.size(); ++i)
		{
			m_buttons[i]->Pos() = (Vector3(MAIN->GetWidth() / 3.0f, (MAIN->GetHeight()) - (i * 100.0f + 300.0f), 1.0f));

			if (m_buttons[i]->CheckMouseCollision(mousePos))
			{
				m_buttons[i]->OnHover(true);

				if (KEY_DOWN(VK_LBUTTON))
				{
					m_buttons[i]->OnClicked();
					m_isSubMenuOn = false;
				}
				continue;
			}

			m_buttons[i]->OnHover(false);
		}
	}
}

void SubMenu::Render()
{
	if (m_isSubMenuOn)
	{
		m_valueBuffer->SetPS(10);
		m_backGround->PSSet(20);

		Quad::UpdateWorld();
		Quad::Render();

		ENV->PostSet();

		for (Button* button : m_buttons)
			button->Render();

		ENV->Set();
	}
}

void SubMenu::GUIRender()
{
	for (Button* button : m_buttons)
		button->GUIRender();
}

void SubMenu::MakeMesh()
{
	float left = -MAIN->GetWidth() * 0.5f;
	float right = +MAIN->GetWidth() * 0.5f;
	float top = +MAIN->GetHeight() * 0.5f;
	float bottom = -MAIN->GetHeight() * 0.5f;

	vector<VertexType>& vertices = m_mesh->GetVertices();

	vertices.emplace_back(left, top, 0, 0, 0);
	vertices.emplace_back(right, top, 0, 1, 0);
	vertices.emplace_back(left, bottom, 0, 0, 1);
	vertices.emplace_back(right, bottom, 0, 1, 1);

	vector<UINT>& indices = m_mesh->GetIndices();
	indices = { 0, 1, 2, 2, 1, 3 };
}
