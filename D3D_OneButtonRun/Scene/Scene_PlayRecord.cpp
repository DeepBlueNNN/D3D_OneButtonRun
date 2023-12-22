#include "framework.h"
#include "Scene_PlayRecord.h"

Scene_PlayRecord::Scene_PlayRecord()
{
	m_sceneName = "PlayRecord";

	// Font 세팅
	Font::Get()->AddColor("Black", 0, 0, 0);
	Font::Get()->AddStyle("Record_Small", L"배달의민족 주아", 40.0f, DWRITE_TEXT_ALIGNMENT_CENTER);
	Font::Get()->AddStyle("Record_Medium", L"배달의민족 주아", 50.0f, DWRITE_TEXT_ALIGNMENT_CENTER);
	Font::Get()->AddStyle("Record_Large", L"배달의민족 주아", 80.0f, DWRITE_TEXT_ALIGNMENT_CENTER);

	// 세이브맵 이름들 가져오기
	StringPath::GetFiles(m_stageNames, "Saved/", "*.xml", false);

	// Textures
	UITexture* texture = new UITexture(L"Textures/UI/Texture_Stage_2.png", Vector2(200.0f, 80.0f));
	m_textures.push_back(texture);

	texture = new UITexture(L"Textures/UI/Texture_Count.png", Vector2(200.0f, 80.0f));
	m_textures.push_back(texture);

	texture = new UITexture(L"Textures/UI/Texture_Time.png", Vector2(200.0f, 80.0f));
	m_textures.push_back(texture);

	// Buttons
	Button* button = new Button(L"Textures/UI/Button_Backward_N.png");
	button->SetTag("Backward");
	button->SetHoverEffct(L"Textures/UI/Button_Backward_Y.png", 1.0f);
	button->SetAction(Button::CHANGESCENE, "MainMenu");
	m_buttons.push_back(button);

	button = new Button(L"Textures/UI/Button_Credit_N.png");
	button->SetTag("Credit");
	button->SetHoverEffct(L"Textures/UI/Button_Credit_Y.png", 1.0f);
	button->SetAction(Button::MAX);
	m_buttons.push_back(button);

	UpdateRecords();
}

Scene_PlayRecord::~Scene_PlayRecord()
{
	for (UITexture* texture : m_textures)
		SAFE_DELETE(texture);
	m_textures.erase(m_textures.begin(), m_textures.end());

	for (Button* button : m_buttons)
		SAFE_DELETE(button);
	m_buttons.erase(m_buttons.begin(), m_buttons.end());
}

void Scene_PlayRecord::Update()
{
	m_textures[0]->Pos() = Vector3((MAIN->GetWidth() / 6.0f), (MAIN->GetHeight() * (4.0f / 5.0f)), 1.0f);
	m_textures[1]->Pos() = Vector3((MAIN->GetWidth() / 6.0f), (MAIN->GetHeight() * (3.0f / 5.0f)), 1.0f);
	m_textures[2]->Pos() = Vector3((MAIN->GetWidth() / 6.0f), (MAIN->GetHeight() * (2.0f / 5.0f)), 1.0f);

	m_buttons[0]->Pos() = Vector3((MAIN->GetWidth() * (1.0f / 3.0f)), (MAIN->GetHeight() * (1.0f / 5.0f)), 1.0f);
	m_buttons[1]->Pos() = Vector3((MAIN->GetWidth() * (2.0f / 3.0f)), (MAIN->GetHeight() * (1.0f / 5.0f)), 1.0f);

	for (int i = 0; i < m_buttons.size(); ++i)
	{
		if (m_buttons[i]->GetPressability() && m_buttons[i]->CheckMouseCollision(mousePos))
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

void Scene_PlayRecord::Render()
{
}

void Scene_PlayRecord::ChangeScene()
{
	UpdateRecords();
	SCENEMANAGER->SetGUIActive(false);
}

void Scene_PlayRecord::PostRender()
{
	ENV->PostSet();

	for (UITexture* texture : m_textures)
		texture->Render();

	for (Button* button : m_buttons)
		button->Render();

	for (int i = 0; i < m_stageNames.size(); ++i)
	{
		PrintRecords(i);
	}

	ENV->Set();
}

void Scene_PlayRecord::GUIRender()
{
}

void Scene_PlayRecord::UpdateRecords()
{
	vector<Record> records = SAVELOAD->GetRecords();

	m_recordCounts.clear();
	m_recordTimes.clear();

	for (auto record : records)
	{
		ConvertRecordToString(record.count, record.time);
	}
}

void Scene_PlayRecord::PrintRecords(int index)
{
	Font::Get()->GetDC()->BeginDraw();

	Font::Get()->SetColor("Black");

	if (m_recordCounts.size() - 1 < index)
	{
		Font::Get()->SetStyle("Record_Medium");
		Font::Get()->RenderText(StringPath::GetFileNameWithoutExtension(m_stageNames[index]).c_str(), { ((float)MAIN->GetWidth() * ((index + 2) / 6.0f)), ((float)MAIN->GetHeight() * (4.0f / 5.0f)) }, Float2(200.0f, 80.0f));
		Font::Get()->SetStyle("Record_Large");
		Font::Get()->RenderText(to_string(0).c_str(), { ((float)MAIN->GetWidth() * ((index + 2) / 6.0f)), ((float)MAIN->GetHeight() * (3.0f / 5.0f)) }, Float2(200.0f, 80.0f));
		Font::Get()->SetStyle("Record_Small");
		Font::Get()->RenderText(to_string(0).c_str(), { ((float)MAIN->GetWidth() * ((index + 2) / 6.0f)), ((float)MAIN->GetHeight() * (2.0f / 5.0f)) }, Float2(200.0f, 80.0f));
	}
	else
	{
		Font::Get()->SetStyle("Record_Medium");
		Font::Get()->RenderText(StringPath::GetFileNameWithoutExtension(m_stageNames[index]).c_str(), { ((float)MAIN->GetWidth() * ((index+2) / 6.0f)), ((float)MAIN->GetHeight() * (4.0f / 5.0f)) }, Float2(200.0f, 80.0f));
		Font::Get()->SetStyle("Record_Large");
		Font::Get()->RenderText(m_recordCounts[index].c_str(), { ((float)MAIN->GetWidth() * ((index+2) / 6.0f)), ((float)MAIN->GetHeight() * (3.0f / 5.0f)) }, Float2(200.0f, 80.0f));
		Font::Get()->SetStyle("Record_Small");
		Font::Get()->RenderText(m_recordTimes[index].c_str(), { ((float)MAIN->GetWidth() * ((index+2) / 6.0f)), ((float)MAIN->GetHeight() * (2.0f / 5.0f)) }, Float2(200.0f, 80.0f));
	}

	Font::Get()->GetDC()->EndDraw();
}

void Scene_PlayRecord::ConvertRecordToString(UINT count, float time)
{
	int temp = (int)(time * 100);
	int milliSecond = temp % 100;
	int second = temp / 100;
	int minute = second / 60;
	second = second % 60;
	int hour = minute / 60;
	minute = minute % 60;
	int day = hour / 24;
	hour = hour % 24;

	string timeRecordTemp = (day > 10 ? to_string(day) : ("0" + to_string(day))) + " : ";
	timeRecordTemp += (hour > 10 ? to_string(hour) : ("0" + to_string(hour))) + " : ";
	timeRecordTemp += (minute > 10 ? to_string(minute) : ("0" + to_string(minute))) + " : ";
	timeRecordTemp += (second > 10 ? to_string(second) : ("0" + to_string(second))) + " : ";
	timeRecordTemp += (milliSecond > 10 ? to_string(milliSecond) : ("0" + to_string(milliSecond)));

	m_recordTimes.push_back(timeRecordTemp);

	m_recordCounts.push_back(to_string(count));

	//cout << timeRecordTemp << '\n';
	//cout << time << '\n';
}
