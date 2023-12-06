#include "framework.h"
#include "Scene_GamePlay.h"

Scene_GamePlay::Scene_GamePlay()
{
	m_sceneName = "GamePlay";

	m_player = new GamePlayer();
	m_player->SetPosition(Vector3(0, 50, 0));
	m_playerOriginPos = m_player->GetColliderPosition();
	m_player->SetIsGravityActive(true);

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
}

Scene_GamePlay::~Scene_GamePlay()
{
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
		m_player->SetPosition(m_playerOriginPos);
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
}

void Scene_GamePlay::GUIRender()
{
	m_player->GUIRender();

	for (BoxCollider* ground : m_grounds)
		ground->GUIRender();
}
