#include "framework.h"
#include "Scene_GamePlay.h"

Scene_GamePlay::Scene_GamePlay()
{
	// Font 세팅
	/*Font::Get()->AddColor("White", 1, 1, 1);
	Font::Get()->AddStyle("Default", L"배달의민족 주아", 80.0f);

	Font::Get()->SetColor("White");
	Font::Get()->SetStyle("Default");*/

	// Camera 임시 세팅
	CAMERA->Pos() = Vector3(13.0f, 71.0f, -170.0f);
	CAMERA->Rot() = Vector3(XMConvertToRadians(24.0f), XMConvertToRadians(3.8f), XMConvertToRadians(0.0f));

	// GamePlay 세팅
	m_sceneName = "GamePlay";

	m_player = new GamePlayer();
	m_player->SetPosition(Vector3(0, 30, 0));
	m_playerOriginPos = m_player->GetColliderPosition();
	m_player->SetIsGravityActive(true);

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

	// UI 변수 세팅
	m_refreshCount = 0;
	m_playTime = 0.0f;
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
}

void Scene_GamePlay::Update()
{
	m_playTime += DELTA;

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

void Scene_GamePlay::Render()
{
	for (Cube* cube : m_groundCubes)
		cube->Render();

	for (BoxCollider* ground : m_grounds)
		ground->Render();

	m_player->Render();
}

void Scene_GamePlay::PreRender()
{
}

void Scene_GamePlay::PostRender()
{
	PrintRefreshCount();
	PrintPlayTime();

	/*Font::Get()->GetDC()->BeginDraw();
	{
		PrintRefreshCount();
		PrintPlayTime();
	}
	Font::Get()->GetDC()->EndDraw();*/
}

void Scene_GamePlay::GUIRender()
{
	m_player->GUIRender();

	for (BoxCollider* ground : m_grounds)
		ground->GUIRender();
}

void Scene_GamePlay::PrintRefreshCount()
{
	bool temp = true;
	ImGui::SetNextWindowPos(ImVec2((float)MAIN->GetWidth() / 2, (float)MAIN->GetHeight() - 100), 0, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin("RefreshCount", &temp, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
	{
		ImGui::TextColored(ImVec4(1, 1, 1, 1), to_string(m_refreshCount).c_str());
		ImGui::SetWindowFontScale(3.5f);
		ImGui::SetWindowSize("RefreshCount", ImVec2(0, 0));
	}
	ImGui::End();

	//Font::Get()->RenderText(to_string(m_refreshCount), { (float)MAIN->GetWidth() / 2, 100 }, Float2(200, 100));
}

void Scene_GamePlay::PrintPlayTime()
{
	bool temp = true;
	ImGui::SetNextWindowPos(ImVec2((float)MAIN->GetWidth() / 2, 100), 0, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin("PlayTime", &temp, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
	{
		ImGui::TextColored(ImVec4(1, 1, 1, 1), ConvertToString(m_playTime).c_str());
		ImGui::SetWindowFontScale(3.5f);
		ImGui::SetWindowSize("PlayTime", ImVec2(0, 0));
	}
	ImGui::End();
	
	//Font::Get()->RenderText(ConvertToString(m_playTime), {((float)MAIN->GetWidth() / 2) + 90, (float)MAIN->GetHeight() - 100});
}

string Scene_GamePlay::ConvertToString(float value)
{
	double playTime = static_cast<double>(m_playTime);
	const auto Len = static_cast<size_t>(_CSTD _scprintf("%.2f", playTime));
	string Str(Len, '\0');
	_CSTD sprintf_s(&Str[0], Len + 1, "%.2f", playTime);

	return Str;
}
