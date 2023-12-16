#include "framework.h"
#include "Scene_GamePlay.h"

Scene_GamePlay::Scene_GamePlay()
{
	// Font ����
	Font::Get()->AddColor("White", 1, 1, 1);
	Font::Get()->AddStyle("Default", L"����ǹ��� �־�", 80.0f);

	Font::Get()->SetColor("White");
	Font::Get()->SetStyle("Default");

	// GamePlay ����
	m_sceneName = "GamePlay";

	m_player = new GamePlayer();
	m_player->SetPosition(Vector3(0, 30, 0));
	m_playerOriginPos = m_player->GetColliderPosition();
	m_playerOriginVelocity = m_player->Velocity();
	m_player->SetIsGravityActive(false);

	// �ٴ� ������Ʈ �ӽ� ����
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

	// UI ���� ����
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
	Font::Get()->Delete();

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
	// Camera �ӽ� ����
	CAMERA->Pos() = Vector3(13.0f, 71.0f, -170.0f);
	CAMERA->Rot() = Vector3(XMConvertToRadians(24.0f), XMConvertToRadians(3.8f), XMConvertToRadians(0.0f));

	if (m_howToPlay->IsPanelOn())
		m_howToPlay->Update();

	m_subMenu->Update();

	if (m_isPlayStart)	// Play Start �ÿ� 3�� ī��Ʈ
	{
		m_player->SetIsGravityActive(false);

		// ���̺�� �� ����
		//vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
		//for (const auto actor : actors)
		//	actor->Update();

		for (Cube* cube : m_groundCubes)
			cube->UpdateWorld();

		m_player->Update();

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
	else	// 3�� ī��Ʈ �Ŀ� Game Play
	{
		if (m_subMenu->IsPanelOn())
		{
			m_player->SetIsGravityActive(false);
			m_isPlaying = false;
		}
		else
		{
			m_player->SetIsGravityActive(true);
			m_isPlaying = true;
		}

		if (m_isPlaying)
		{
			m_playTime += DELTA;

			// ���̺�� �� ����
			//vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
			//for (const auto actor : actors)
			//	actor->Update();

			for (Cube* cube : m_groundCubes)
				cube->UpdateWorld();

			m_player->Update();

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

			if (KEY_DOWN(VK_SPACE))
			{
				m_player->SetPosition(m_playerOriginPos);
				m_refreshCount++;
			}
		}
	}
}

void Scene_GamePlay::Render()
{
	// ���̺�� �� ����
	//vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
	//for (const auto actor : actors)
	//	actor->Render();

	for (Cube* cube : m_groundCubes)
		cube->Render();

	for (BoxCollider* ground : m_grounds)
		ground->Render();

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
	m_player->GUIRender();

	for (BoxCollider* ground : m_grounds)
		ground->GUIRender();
}

void Scene_GamePlay::Initialize()
{
	// Camera �ӽ� ����
	CAMERA->Pos() = Vector3(13.0f, 71.0f, -170.0f);
	CAMERA->Rot() = Vector3(XMConvertToRadians(24.0f), XMConvertToRadians(3.8f), XMConvertToRadians(0.0f));

	// Player ��ġ, velocity ����
	m_player->SetPosition(m_playerOriginPos);
	m_player->Velocity() = m_playerOriginVelocity;

	// Game Play �ʱⰪ ����
	m_refreshCount = 0;
	m_playTime = 0.0f;
	m_playStartCount = 3;

	// Game Play flag ����
	m_isPlayStart = true;
	m_isPlaying = false;

	m_howToPlay->SetPanelOn(true);
}

void Scene_GamePlay::PrintRefreshCount()
{
	/*bool temp = true;
	ImGui::SetNextWindowPos(ImVec2((float)MAIN->GetWidth() / 2, (float)MAIN->GetHeight() - 100), 0, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin("RefreshCount", &temp, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
	{
		ImGui::TextColored(ImVec4(1, 1, 1, 1), to_string(m_refreshCount).c_str());
		ImGui::SetWindowFontScale(3.5f);
		ImGui::SetWindowSize("RefreshCount", ImVec2(0, 0));
	}
	ImGui::End();*/

	Font::Get()->RenderText(to_string(m_refreshCount), { (float)MAIN->GetWidth() / 2 + 50, 100 }, Float2(200, 100));
}

void Scene_GamePlay::PrintPlayTime()
{
	/*bool temp = true;
	ImGui::SetNextWindowPos(ImVec2((float)MAIN->GetWidth() / 2, 100), 0, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin("PlayTime", &temp, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
	{
		ImGui::TextColored(ImVec4(1, 1, 1, 1), ConvertToString(m_playTime).c_str());
		ImGui::SetWindowFontScale(3.5f);
		ImGui::SetWindowSize("PlayTime", ImVec2(0, 0));
	}
	ImGui::End();*/
	
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
