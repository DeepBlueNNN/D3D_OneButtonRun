#include "framework.h"
#include "Scene_MapEditor.h"

Scene_MapEditor::Scene_MapEditor()
{
	vector<GameActor*>& actors = SAVELOAD->GetGameActors();
	vector<InstancingActor*>& instancingActors = SAVELOAD->GetInstancingActors();

	GamePlayer* player = SAVELOAD->GetPlayer();
	player->SetPosition(Vector3(0, 50, 0));

	int index;

	InstancingActor* ground1 = new InstancingActor(GameActor::GameActorTag::GROUND, "Ground_01", Collider::BOX);
	index = ground1->Add();
	ground1->GetModels()->GetTransforms()[index]->Pos() = Vector3(0, -5, 0);
	ground1->GetColliders()[index]->Pos() = Vector3(0, -5, 0);
	instancingActors.push_back(ground1);

	InstancingActor* rock1 = new InstancingActor(GameActor::GameActorTag::ROCK, "Rock_01", Collider::SPHERE);
	index = rock1->Add();
	rock1->GetModels()->GetTransforms()[index]->Pos() = Vector3(-10, 0, 0);
	rock1->GetColliders()[index]->Pos() = Vector3(-10, 0, 0);
	instancingActors.push_back(rock1);

	InstancingActor* tree1 = new InstancingActor(GameActor::GameActorTag::TREE, "Tree_01", Collider::CAPSULE);
	index = tree1->Add();
	tree1->GetModels()->GetTransforms()[index]->Pos() = Vector3(10, 0, 0);
	tree1->GetColliders()[index]->Pos() = Vector3(10, 0, 0);
	instancingActors.push_back(tree1);

	InstancingActor* tree2 = new InstancingActor(GameActor::GameActorTag::TREE, "Tree_02", Collider::CAPSULE);
//	tree2->SetPosition(Vector3(-10, 3, 0));
	index = tree2->Add();
	tree2->GetModels()->GetTransforms()[index]->Pos() = Vector3(20, 0, 0);
	tree2->GetColliders()[index]->Pos() = Vector3(20, 0, 0);
	index = tree2->Add();
	tree2->GetModels()->GetTransforms()[index]->Pos() = Vector3(0, 0, 0);
	tree2->GetColliders()[index]->Pos() = Vector3(0, 0, 0);
	index = tree2->Add();
	tree2->GetModels()->GetTransforms()[index]->Pos() = Vector3(-20, 0, 0);
	tree2->GetColliders()[index]->Pos() = Vector3(-20, 0, 0);
	instancingActors.push_back(tree2);

	//GameActor* tree = new Tree("Tree_02");
	//tree->SetPosition(Vector3(0, 3, 0));
	//actors.push_back(tree);

	//GameActor* rock = new Rock("Rock_01");
	//rock->SetPosition(Vector3(-5, 1, 0));
	//actors.push_back(rock);
}

Scene_MapEditor::~Scene_MapEditor()
{
}

void Scene_MapEditor::Update()
{
	vector<InstancingActor*> actors = SAVELOAD->GetInstancingActors();
	for (const auto actor : actors)
		actor->Update();

	SAVELOAD->GetPlayer()->Update();

	//SelectActor();
}

void Scene_MapEditor::Render()
{
	vector<InstancingActor*> actors = SAVELOAD->GetInstancingActors();
	for (const auto actor : actors)
		actor->Render();

	SAVELOAD->GetPlayer()->Render();
}

void Scene_MapEditor::PreRender()
{
}

void Scene_MapEditor::PostRender()
{
}

void Scene_MapEditor::GUIRender()
{
	vector<GameActor*>& actors = SAVELOAD->GetGameActors();

	// m_selectedActor == GameActor*
	//// SelectedActor::GUIRender
	//string label = "Selected Actor";
	//if (ImGui::TreeNode(label.c_str()))
	//{
	//	if (m_selectedActor)
	//	{
	//		m_selectedActor->GUIRender();
	//	}
	//	DeleteActor(m_selectedActor);

	//	ImGui::TreePop();
	//}

	//label = "Actors";
	//if (ImGui::TreeNode(label.c_str()))
	//{
	//	for (const auto actor : actors)
	//		actor->GUIRender();

	//	ImGui::TreePop();
	//}

	//AddActor();

	SAVELOAD->GUIRender();
}

void Scene_MapEditor::SelectActor()
{
	Ray ray = CAMERA->ScreenPointToRay(mousePos);
	Contact contact;
	vector<GameActor*>& actors = SAVELOAD->GetGameActors();
	for (const auto actor : actors)
	{
		for (int i = 0; i < dynamic_cast<InstancingActor*>(actor)->GetColliders().size(); i++)
		{
			if (dynamic_cast<InstancingActor*>(actor)->GetColliders()[i]->IsRayCollision(ray, &contact)
				&& KEY_PRESS(VK_LBUTTON))
			{
				m_selectedActor = dynamic_cast<InstancingActor*>(actor)->GetModels()->GetTransforms()[i];
				dynamic_cast<InstancingActor*>(actor)->GetColliders()[i]->SetColor(Float4(0, 1, 0, 1));
			}
		}

		// actor == GameActor*
		//if (m_selectedActor != actor)
		//	actor->GetCollider()->SetColor(Float4(0, 1, 0, 1));

		//if (actor->GetCollider()->IsRayCollision(ray, &contact) && KEY_PRESS(VK_LBUTTON))
		//{
		//	m_selectedActor = actor;
		//	actor->GetCollider()->SetColor(Float4(1, 0, 0, 1));
		//	break;
		//}

		//if (!actor->GetCollider()->IsRayCollision(ray, &contact) && KEY_PRESS(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
		//	m_selectedActor = nullptr;
	}
}

void Scene_MapEditor::AddActor()
{
	string label = "Actor List";
	bool ret[static_cast<int>(GameActor::GameActorTag::MAX)] = { false };
	if (ImGui::TreeNode(label.c_str()))
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));
		int classIndex;
		for (classIndex = 0; classIndex < GameActor::GameActorTag::MAX; classIndex++)
		{
			ret[classIndex] = ImGui::Button(SAVELOAD->GetClassNames()[classIndex].c_str(), ImVec2(100, 20));
			if (ret[classIndex])
				break;
		}
		ImGui::PopStyleColor(3);

		// 추가할 엑터가 없는 경우 return
		if (classIndex >= static_cast<int>(GameActor::GameActorTag::MAX))
		{
			ImGui::TreePop();
			return;
		}

		vector<GameActor*>& actors = SAVELOAD->GetGameActors();
		FileDialog dialog;
		wstring file;
		if (dialog.Open(file,FileDialog::FBX))
		{
			file = StringPath::ToWString(StringPath::GetFileNameWithoutExtension(StringPath::ToString(file)));

			//GameActor* actor = SAVELOAD->CreateActor(static_cast<GameActor::GameActorTag>(i),
			//										 file, )
		}

		ImGui::TreePop();
	}
}

void Scene_MapEditor::DeleteActor(GameActor* actor)
{
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.8f, 0.8f));
	bool ret = ImGui::Button(u8"삭제", ImVec2(100, 20));
	ImGui::PopStyleColor(3);

	if (ret)
	{
		vector<GameActor*>& actors = SAVELOAD->GetGameActors();
		for (int i = 0; i < actors.size(); i++)
		{
			if (actors[i] == actor)
			{
				actors.erase(actors.begin() + i);
				break;
			}
		}
	}
}
