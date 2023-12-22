#include "framework.h"
#include "Scene_SelectStage.h"

Scene_SelectStage::Scene_SelectStage()
{
	m_sceneName = "SelectStage";

	UpdateMaps();
}

Scene_SelectStage::~Scene_SelectStage()
{
	for (Button* button : m_buttons)
		SAFE_DELETE(button);
	m_buttons.erase(m_buttons.begin(), m_buttons.end());
}

void Scene_SelectStage::Update()
{
	for (int i = 0; i < m_buttons.size(); ++i)
	{
		if (i == m_buttons.size() - 1)
			m_buttons[i]->Pos() = (Vector3((MAIN->GetWidth() * 0.5f), (MAIN->GetHeight() / 5.0f), 1.0f));
		else
			m_buttons[i]->Pos() = (Vector3((MAIN->GetWidth() / 5.0f) + (i * 350.0f), (MAIN->GetHeight() / 2.0f) + 50.0f, 1.0f));

		// ���� ���������� Ŭ�����ؾ� ���� �������� ���� ����
		if (m_buttons[i]->GetPressability() && m_buttons[i]->CheckMouseCollision(mousePos))
		{
			m_buttons[i]->OnHover(true);
			if (KEY_DOWN(VK_LBUTTON))
			{
				if (i < m_buttons.size() - 1)
					SAVELOAD->SetLoadFile(i, StringPath::ToWString(m_stageNames[i]));

				m_buttons[i]->OnClicked();
			}
			continue;
		}

		m_buttons[i]->OnHover(false);
	}
}

void Scene_SelectStage::ChangeScene()
{
	UpdateMaps();
	SCENEMANAGER->SetGUIActive(false);
}

void Scene_SelectStage::PostRender()
{
	ENV->PostSet();

	for (Button* button : m_buttons)
	{
		button->Render();
	}

	ENV->Set();
}

void Scene_SelectStage::GUIRender()
{
	for (Button* button : m_buttons)
		button->GUIRender();
}

/// <summary>
/// ���� ���� �ʱ�ȭ �� ����� ���̺�� ���� �̸��� ����� �޾ƿ���
/// </summary>
void Scene_SelectStage::UpdateMaps()
{
	for (Button* button : m_buttons)
		SAFE_DELETE(button);
	m_buttons.erase(m_buttons.begin(), m_buttons.end());
	m_stageNames.erase(m_stageNames.begin(), m_stageNames.end());
	m_thumbnails.erase(m_thumbnails.begin(), m_thumbnails.end());

	// ���̺�� �̸��� ��������
	StringPath::GetFiles(m_stageNames, "Saved/", "*.xml", false);
	StringPath::GetFiles(m_thumbnails, L"Textures/Thumbnails/", L"*.png", false);
	wstring thumbnailName = L"";

	// ���̺�� �� ��ŭ ��ư ����
	for (int i = 0; i <= m_stageNames.size(); ++i)
	{
		// Ŭ���� ����� ������ ���� ������̹��� ����
		if (i == 0 || i <= SAVELOAD->GetRecords().size())
			thumbnailName = m_thumbnails[i];
		else
			thumbnailName = L"Textures/Thumbnails/Question.png";

		Button* stageButton = new Button(thumbnailName, Vector2(300.0f, 400.0f));
		stageButton->SetOutline(0.01f);
		stageButton->SetHoverEffct(L"Textures/Color/Green.png", 0.1f);
		stageButton->SetAction(Button::CHANGESCENE, "GamePlay");
		stageButton->SetPressable(false);

		m_buttons.push_back(stageButton);
	}

	Button* backward = new Button(L"Textures/UI/Button_Backward_N.png");
	backward->SetHoverEffct(L"Textures/UI/Button_Backward_Y.png", 1.0f);
	backward->SetAction(Button::CHANGESCENE, "MainMenu");
	m_buttons.push_back(backward);

	for (int i = 0; i < m_buttons.size(); ++i)
	{
		// ���� ���������� Ŭ�����ؾ� ���� �������� ���� ����
		if (i <= SAVELOAD->GetRecords().size())
		{
			m_buttons[i]->SetPressable(true);
		}
	}
}
