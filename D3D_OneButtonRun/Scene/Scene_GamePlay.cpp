#include "framework.h"
#include "Scene_GamePlay.h"

Scene_GamePlay::Scene_GamePlay()
{
	m_player = new GamePlayer();
	m_player->SetPosition(Vector3(0, 50, 0));
//	m_player->CurPos() = m_player->GetColliderPosition();

	m_ground = new BoxCollider(Vector3(100.0f, 10.0f, 100.0f));
	m_ground->Pos() = Vector3(0.0f, 0.0f, 0.0f);
	m_ground->Rot() = Vector3(0.0f, 0.0f, XMConvertToRadians(-30.0f));
}

Scene_GamePlay::~Scene_GamePlay()
{
	SAFE_DELETE(m_ground);
	SAFE_DELETE(m_player);
}

void Scene_GamePlay::Update()
{
	m_player->Update();

	if (m_ground->IsCollision(m_player->GetCollider()))
	{
		m_player->Friction(m_ground->GetClosestPoint());
		m_player->SetIsCollision(true);
	}
	else
	{
		m_player->SetIsCollision(false);
	}
}

void Scene_GamePlay::Render()
{
	m_ground->Render();
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
	m_ground->GUIRender();
}
