#include "framework.h"
#include "Scene_GamePlay.h"

Scene_GamePlay::Scene_GamePlay()
{
	// Font 세팅
	Font::Get()->AddColor("White", 1, 1, 1);
	Font::Get()->AddStyle("Default", L"배달의민족 주아", 80.0f);

	Font::Get()->SetColor("White");
	Font::Get()->SetStyle("Default");

	// GamePlay 세팅
	m_sceneName = "GamePlay";

	m_player = new GamePlayer();
	m_player->SetPosition(Vector3(0, 30, 0));
	m_playerOriginPos = m_player->GetColliderPosition();
	m_playerOriginVelocity = m_player->Velocity();
	m_player->SetIsGravityActive(false);

	// 바닥 오브젝트 임시 세팅
	m_grounds.push_back(new BoxCollider(Vector3(100.0f, 10.0f, 100.0f)));
	m_grounds.back()->Pos() = Vector3(0.0f, 0.0f, 0.0f);
	m_grounds.back()->Rot() = Vector3(0.0f, 0.0f, XMConvertToRadians(-45.0f));
	m_grounds.back()->SetTag("ground_01");

	m_grounds.push_back(new BoxCollider(Vector3(100.0f, 10.0f, 100.0f)));
	m_grounds.back()->Pos() = Vector3(30.0f, 0.0f, 0.0f);
	m_grounds.back()->Rot() = Vector3(0.0f, 0.0f, XMConvertToRadians(30.0f));
	m_grounds.back()->SetTag("ground_02");

	m_groundCubes.push_back(new Cube());
	m_groundCubes.back()->Scale() = Vector3(50.0f, 5.0f, 50.0f);
	m_groundCubes.back()->Pos() = Vector3(0.0f, 0.0f, 0.0f);
	m_groundCubes.back()->Rot() = Vector3(0.0f, 0.0f, XMConvertToRadians(-45.0f));

	m_groundCubes.push_back(new Cube());
	m_groundCubes.back()->Scale() = Vector3(50.0f, 5.0f, 50.0f);
	m_groundCubes.back()->Pos() = Vector3(30.0f, 0.0f, 0.0f);
	m_groundCubes.back()->Rot() = Vector3(0.0f, 0.0f, XMConvertToRadians(30.0f));

	// Target Actor 임시 세팅
	m_targetActor = new TargetActor();
	m_targetActor->SetPosition(Vector3(50, 20, 0));

	// UI 변수 세팅
	m_playStartCount = 3;
	m_refreshCount = 0;
	m_playTime = 0.0f;

	// UIPanel
	m_subMenu = new SubMenu();
	m_howToPlay = new HowToPlay();
	m_howToPlay->SetPanelOn(true);
}

Scene_GamePlay::~Scene_GamePlay()
{
	SAFE_DELETE(m_targetActor);

	for (Cube* cube : m_groundCubes)
		SAFE_DELETE(cube);
	m_groundCubes.erase(m_groundCubes.begin(), m_groundCubes.end());

	for (BoxCollider* ground : m_grounds)
		SAFE_DELETE(ground);
	m_grounds.erase(m_grounds.begin(), m_grounds.end());

	SAFE_DELETE(m_player);
	SAFE_DELETE(m_howToPlay);
	SAFE_DELETE(m_subMenu);
}

void Scene_GamePlay::Update()
{
	// Camera 임시 세팅
	CAMERA->Pos() = Vector3(13.0f, 71.0f, -170.0f);
	CAMERA->Rot() = Vector3(XMConvertToRadians(24.0f), XMConvertToRadians(3.8f), XMConvertToRadians(0.0f));

	if (m_howToPlay->IsPanelOn())
		m_howToPlay->Update();

	m_subMenu->Update();

	if (m_isPlayStart)	// Play Start 시에 3초 카운트
	{
		m_player->SetIsGravityActive(false);

		if (!m_subMenu->IsPanelOn())
		{
			m_howToPlay->SetPanelOn(true);

			m_playTime += DELTA;
			if (m_playTime >= 1.0f)
			{
				m_playStartCount -= (UINT)m_playTime;
				m_playTime = 0.0f;
			}

			if (m_playStartCount <= 0)
			{
				m_isPlayStart = false;
				m_isPlaying = true;

				m_howToPlay->SetPanelOn(false);
			}
		}
		else
			m_howToPlay->SetPanelOn(false);
	}
	else	// 3초 카운트 후에 Game Play
	{
		if (!m_isStageClear)
		{
			m_isPlaying = m_subMenu->IsPanelOn() ? false : true;
			m_player->SetIsGravityActive(m_isPlaying);
		}

		if (m_isPlaying)
		{
			m_playTime += DELTA;

			// Ground Check
			for (BoxCollider* ground : m_grounds)
			{
				/*if (ground->IsCollision(m_player->GetCollider()))
				{
					m_player->Friction(ground->GetClosestPoint());
					m_player->SetIsCollision(true);
				}
				else
				{
					m_player->SetIsCollision(false);
				}*/

				if (m_player->GetCollider()->IsCollision(ground))
				{
					m_player->Friction(ground->GetClosestPoint());
					m_player->RegisterObject(ground);
				}
				else
				{
					m_player->RemoveObject(ground);
				}
			}

			// Player Position Refresh
			if (KEY_DOWN(VK_SPACE))
			{
				m_player->SetPosition(m_playerOriginPos);
				m_refreshCount++;
			}

			// Target Actor & Player Collision Check
			if (m_targetActor->GetCollider()->IsCollision(m_player->GetCollider()))
			{
				printf("Target & Player Collision True \n");

				RecordSave();

				m_player->SetIsGravityActive(false);
				m_isPlaying = false;
				m_isStageClear = true;
			}
		}
	}

	if (m_isStageClear)
	{
		// 게임 클리어 화면(Panel) 출력...

		if (KEY_DOWN('Y'))
		{
			// Go to Stage Select Scene...
		}
	}

	// 세이브된 맵 적용
			//vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
			//for (const auto actor : actors)
			//	actor->Update();

	for (Cube* cube : m_groundCubes)
		cube->UpdateWorld();

	m_targetActor->Update();
	m_player->Update();
}

void Scene_GamePlay::Render()
{
	// 세이브된 맵 적용
	//vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
	//for (const auto actor : actors)
	//	actor->Render();

	for (Cube* cube : m_groundCubes)
		cube->Render();

	for (BoxCollider* ground : m_grounds)
		ground->Render();

	m_targetActor->Render();
	m_player->Render();
}

void Scene_GamePlay::ChangeScene()
{
	Initialize();
}

void Scene_GamePlay::PreRender()
{
}

void Scene_GamePlay::PostRender()
{
	if (!m_isStageClear)
	{
		Font::Get()->GetDC()->BeginDraw();
		{
			PrintRefreshCount();
			PrintPlayTime();
		}
		Font::Get()->GetDC()->EndDraw();
	}

	if (m_howToPlay->IsPanelOn())
		m_howToPlay->Render();

	m_subMenu->Render();
}

void Scene_GamePlay::GUIRender()
{
	m_player->GUIRender();
	m_targetActor->GUIRender();

	for (BoxCollider* ground : m_grounds)
		ground->GUIRender();
}

void Scene_GamePlay::Initialize()
{
	// Camera 임시 세팅
	CAMERA->Pos() = Vector3(13.0f, 71.0f, -170.0f);
	CAMERA->Rot() = Vector3(XMConvertToRadians(24.0f), XMConvertToRadians(3.8f), XMConvertToRadians(0.0f));

	// Player 위치, velocity 설정
	m_player->SetPosition(m_playerOriginPos);
	m_player->Velocity() = m_playerOriginVelocity;

	// Game Play 초기값 설정
	m_refreshCount = 0;
	m_playTime = 0.0f;
	m_playStartCount = 3;

	// Game Play flag 설정
	m_isPlayStart = true;
	m_isPlaying = false;

	m_howToPlay->SetPanelOn(true);
}

void Scene_GamePlay::PrintRefreshCount()
{
	Font::Get()->RenderText(to_string(m_refreshCount), { (float)MAIN->GetWidth() / 2 + 50, 100 }, Float2(200, 100));
}

void Scene_GamePlay::PrintPlayTime()
{
	if (m_isPlayStart)
		Font::Get()->RenderText(to_string(m_playStartCount).c_str(), {((float)MAIN->GetWidth() / 2), (float)MAIN->GetHeight() - 100});
	else
		Font::Get()->RenderText(ConvertToString(m_playTime), {((float)MAIN->GetWidth() / 2) + 90, (float)MAIN->GetHeight() - 100});
}

string Scene_GamePlay::ConvertToString(float value)
{
	double playTime = static_cast<double>(m_playTime);
	const auto Len = static_cast<size_t>(_CSTD _scprintf("%.2f", playTime));
	string Str(Len, '\0');
	_CSTD sprintf_s(&Str[0], Len + 1, "%.2f", playTime);

	return Str;
}

void Scene_GamePlay::RecordSave()
{
	int temp = (int)(m_playTime * 100);
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

	string refreshRecordTemp = to_string(m_refreshCount);

	cout << timeRecordTemp << '\n';
	cout << m_playTime << '\n';
}
