#include "framework.h"
#include "Scene_GamePlay.h"

Scene_GamePlay::Scene_GamePlay()
{
	// Font 세팅
	Font::Get()->AddColor("White", 1, 1, 1);
	Font::Get()->AddStyle("Default", L"배달의민족 주아", 80.0f);

	// GamePlay 세팅
	m_sceneName = "GamePlay";

	SAVELOAD->GetPlayer()->SetPosition(SAVELOAD->GetPlayerOriginPos());
	SAVELOAD->GetPlayer()->SetIsGravityActive(false);
	m_playerOriginVelocity = SAVELOAD->GetPlayer()->Velocity();

	// UI 변수 세팅
	m_playStartCount = 3;
	m_refreshCount = 0;
	m_playTime = 0.0f;

	// UIPanel
	m_subMenu = new SubMenu();
	m_howToPlay = new HowToPlay();
	m_howToPlay->SetPanelOn(true);
	m_clearUI = new ClearUI();
	m_clearUI->SetPanelOn(false);
}

Scene_GamePlay::~Scene_GamePlay()
{
	SAFE_DELETE(m_howToPlay);
	SAFE_DELETE(m_subMenu);
	SAFE_DELETE(m_clearUI);
}

void Scene_GamePlay::Update()
{
	if (m_howToPlay->IsPanelOn())
		m_howToPlay->Update();

	m_subMenu->Update();

	// 세이브된 맵 적용
	vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
	for (const auto actor : actors)
		actor->Update();

	if (m_isPlayStart)	// Play Start 시에 3초 카운트
	{
		SAVELOAD->GetPlayer()->GetModelAnim()->PlayClip(17);
		SAVELOAD->GetPlayer()->SetIsGravityActive(false);

		SAVELOAD->GetTargetActor()->Update();
		SAVELOAD->GetPlayer()->Update();

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
			SAVELOAD->GetPlayer()->SetIsGravityActive(m_isPlaying);
		}

		if (m_isPlaying)
		{
			m_playTime += DELTA;

			//SAVELOAD->GetPlayer()->Update();

			// Ground Check
			for (const auto actor : actors)
			{
				for (const auto colliders : actor->GetColliders())
				{
					for (const auto collider : colliders)
					{
						if (SAVELOAD->GetPlayer()->GetCollider()->IsCollision(collider))
						{
							SAVELOAD->GetPlayer()->Friction(collider->GetClosestPoint());
							SAVELOAD->GetPlayer()->RegisterObject(collider);
						}
						else
						{
							SAVELOAD->GetPlayer()->RemoveObject(collider);
						}
					}
				}
			}

			// Player Position Refresh
			if (KEY_DOWN(VK_SPACE))
			{
				SAVELOAD->GetPlayer()->SetPosition(SAVELOAD->GetPlayerOriginPos());
				m_refreshCount++;
			}

			// Target Actor & Player Collision Check
			if (SAVELOAD->GetTargetActor()->GetCollider()->IsCollision(SAVELOAD->GetPlayer()->GetCollider()))
			{
				printf("Target & Player Collision True \n");

				SAVELOAD->SaveRecord(SAVELOAD->GetCurrentStage(), m_refreshCount, m_playTime);

				m_isPlaying = false;
				m_isStageClear = true;
				m_clearUI->SetPanelOn(true);
			}
		}
	}

	// Chit 적용
	if (KEY_PRESS(VK_CONTROL) && KEY_PRESS('K') && KEY_DOWN('C'))
	{
		m_isStageClear = true;
		m_clearUI->SetPanelOn(true);
		SAVELOAD->SaveRecord(SAVELOAD->GetCurrentStage(), m_refreshCount, m_playTime);
	}

	if (m_isStageClear)
	{
		SAVELOAD->GetPlayer()->SetIsGravityActive(false);
		SAVELOAD->GetPlayer()->GetModelAnim()->PlayClip(15);
		// Camera Transform Set
		SAVELOAD->SetCameraClearView();

		Vector3(37.674, -3.857, -23.195);
		Vector3(0.301, 13.127, 0);

		// 클리어 텍스처
		m_clearUI->Update();

		if (KEY_DOWN('Y'))
		{
			SCENEMANAGER->ChangeScene("MainMenu");
		}
	}

	SAVELOAD->GetTargetActor()->Update();
	SAVELOAD->GetPlayer()->Update();
}

void Scene_GamePlay::Render()
{
	// 세이브된 맵 적용
	vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
	for (const auto actor : actors)
		actor->Render();

	SAVELOAD->GetTargetActor()->Render();
	SAVELOAD->GetPlayer()->Render();
}

void Scene_GamePlay::ChangeScene()
{
	Font::Get()->SetColor("White");
	Font::Get()->SetStyle("Default");

	ENV->SetRenderMode(false);
	SCENEMANAGER->SetGUIActive(false);

	Initialize();
}

void Scene_GamePlay::PreRender()
{
}

void Scene_GamePlay::PostRender()
{
	if (m_isStageClear)
	{
		m_clearUI->Render();
	}

	Font::Get()->GetDC()->BeginDraw();
	{
		PrintRefreshCount();
		PrintPlayTime();
	}
	Font::Get()->GetDC()->EndDraw();

	if (m_howToPlay->IsPanelOn())
		m_howToPlay->Render();

	m_subMenu->Render();
}

void Scene_GamePlay::GUIRender()
{
	SAVELOAD->GetPlayer()->GUIRender();
	SAVELOAD->GetTargetActor()->GUIRender();
}

void Scene_GamePlay::Initialize()
{
	SAVELOAD->GetPlayer()->InitTransform();
	// Player 위치, velocity 설정
	SAVELOAD->GetPlayer()->SetPosition(SAVELOAD->GetPlayerOriginPos());
	SAVELOAD->GetPlayer()->SetRotation(SAVELOAD->GetPlayerOriginRot());
	SAVELOAD->GetPlayer()->Velocity() = m_playerOriginVelocity;

	SAVELOAD->GetPlayer()->SetIsGravityActive(false);

	// Game Play 초기값 설정
	m_refreshCount = 0;
	m_playTime = 0.0f;
	m_playStartCount = 3;

	// Game Play flag 설정
	m_isStageClear = false;
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
