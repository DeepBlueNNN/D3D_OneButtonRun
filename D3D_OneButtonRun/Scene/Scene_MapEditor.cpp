#include "framework.h"
#include "Scene_MapEditor.h"

Scene_MapEditor::Scene_MapEditor()
{
	//m_actors.push_back(new GamePlayer());
	//m_actors.push_back(new GamePlayer());
	//m_actors.push_back(new GamePlayer());
}

Scene_MapEditor::~Scene_MapEditor()
{
}

void Scene_MapEditor::Update()
{
	for (const auto actor : m_actors)
		actor->Update();

	//m_actors[0]->SetPosition(Vector3());
	//m_actors[1]->SetPosition(Vector3(-10, 0, 0));
	//m_actors[2]->SetPosition(Vector3(10, 0, 0));

	SelectionActor();
}

void Scene_MapEditor::Render()
{
	for (const auto actor : m_actors)
		actor->Render();
}

void Scene_MapEditor::PreRender()
{
}

void Scene_MapEditor::PostRender()
{
}

void Scene_MapEditor::GUIRender()
{
	string label = "Selected Actor";

	if (ImGui::TreeNode(label.c_str()))
	{
		if(m_selectedActor)
		{
			m_selectedActor->GUIRender();
		}

		ImGui::TreePop();
	}

	dynamic_cast<GamePlayer*>(m_actors[0])->GUIRender();
}

void Scene_MapEditor::SelectionActor()
{
	Ray ray = CAMERA->ScreenPointToRay(mousePos);
	Contact contact;
	for (const auto actor : m_actors)
	{
		if(m_selectedActor != actor)
			actor->GetCollider()->SetColor(Float4(0, 1, 0, 1));

		if (actor->GetCollider()->IsRayCollision(ray, &contact) && KEY_PRESS(VK_LBUTTON))
		{
			m_selectedActor = actor;
			actor->GetCollider()->SetColor(Float4(1, 0, 0, 1));
			break;
		}

		if (!actor->GetCollider()->IsRayCollision(ray, &contact) && KEY_PRESS(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
			m_selectedActor = nullptr;
	}
}
