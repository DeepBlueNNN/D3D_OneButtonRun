#include "framework.h"
#include "Scene_MapEditor.h"

Scene_MapEditor::Scene_MapEditor()
{
	m_sceneName = "MapEditor";

	SAVELOAD->GetPlayer()->SetIsGravityActive(false);

	vector<string> fileList;
	StringPath::GetFiles(fileList, "Assets/FBX/", "*", true);
	for (auto& list : fileList)
	{
		string fbxFolder = "Assets/FBX/";
		list = list.substr(fbxFolder.length());

		// ���� ������ �ִ°�� �и�
		size_t result = list.find('/');
		string folder = list.substr(0, result);
		if (folder.length() > 0)
			m_fbxList.insert(make_pair(folder, vector<string>()));

		// ���� �̸��� ���缭 �����̸� ����
		list = StringPath::GetFileNameWithoutExtension(list);
		m_fbxList.find(folder)->second.push_back(list);
	}

	for (const auto& list : m_fbxList)
	{
		cout << list.first << " : ";
		for (const auto& name : list.second)
			cout << name << ", ";
		cout << '\n';
	}

	WCHAR lpCurrentDirectory[1024];
	GetCurrentDirectory(1024, lpCurrentDirectory);
	m_projectPath = lpCurrentDirectory;
}

Scene_MapEditor::~Scene_MapEditor()
{
}

void Scene_MapEditor::Update()
{
	ENV->Set();
	ENV->Update();

	SAVELOAD->GetPlayer()->Update();

	vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
	if (actors.size() > 0)
	{
		for (const auto actor : actors)
			actor->Update();
	}

	SelectActor();

	if (KEY_PRESS(VK_CONTROL) && KEY_DOWN('S'))
	{
		FileDialog dialog;
		wstring saveFile;

		if (dialog.Save(saveFile, FileDialog::SCENE))
		{
			saveFile = saveFile.substr(m_projectPath.size() + 1, saveFile.size());
			SAVELOAD->SaveScene(saveFile);	// Saved\\Test
		}
	}
}

void Scene_MapEditor::Render()
{
	SAVELOAD->GetPlayer()->Render();

	vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
	if (actors.size() > 0)
	{
		for (const auto actor : actors)
			actor->Render();
	}
}

void Scene_MapEditor::PreRender()
{
}

void Scene_MapEditor::PostRender()
{
}

void Scene_MapEditor::GUIRender()
{
	ENV->GUIRender();
	SAVELOAD->GetSky()->GUIRender();
	vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();

	// SelectedActor's GUIRender
	string label = "Selected Actor";
	if (ImGui::TreeNode(label.c_str()))
	{
		if (m_selectedActor.transform)
		{
			// SelectedActor�� actor, collider ���� �����̱�
			if (ImGui::DragFloat3("Actor Position", (float*)&m_actorPos, 0.1f))
			{ 
				m_selectedActor.transform->Pos() += m_actorPos;
				for (const auto& collider : m_selectedActor.actor->GetColliders()[m_selectedActor.index])
					collider->Pos() += m_actorPos;
				m_actorPos = Vector3();
			}
			//// SelectedActor�� actor, collider�� actor�������� rot
			//if (ImGui::DragFloat3("Actor Rotation", (float*)&m_actorRot, 0.05f))
			//{
			//	m_selectedActor.transform->Rot() += m_actorRot;
			//	for(const auto& collider : m_selectedActor.actor->GetColliders()[m_selectedActor.index])
			//	{
			//		if (abs(m_actorRot.x) >= 0.05f)
			//		{

			//		}

			//		Vector3 delta = collider->Pos() - m_selectedActor.transform->Pos();
			//		delta = Vector3(cosf(XMConvertToRadians(m_actorRot.y)) * -cosf(XMConvertToRadians(m_actorRot.z)) * delta.x, 
			//			sinf(XMConvertToRadians(m_actorRot.x)) * sinf(XMConvertToRadians(m_actorRot.z)) * delta.y,
			//			cosf(XMConvertToRadians(m_actorRot.x)) * sinf(XMConvertToRadians(m_actorRot.y)) * delta.z);
			//		collider->Rot() += m_actorRot;
			//		//collider->Pos() = m_selectedActor.transform->Pos() + delta;
			//		collider->Pos() = m_selectedActor.transform->Pos() + delta;
			//	}
			//	m_actorRot = Vector3();
			//}

			//if (ImGui::DragFloat3("Actor Rotation", (float*)&m_actorRot, 0.05f))
			//{
			//	for (auto& collider : m_selectedActor.actor->GetColliders()[m_selectedActor.index])
			//	{
			//		Vector3 delta = collider->Pos() - m_selectedActor.transform->Pos();
			//		float r = delta.Length();

			//		m_selectedActor.transform->Rot().x = m_actorRot.x;
			//		m_selectedActor.transform->Rot().y = m_actorRot.y;
			//		m_selectedActor.transform->Rot().z = m_actorRot.z;

			//		Vector3 nextPos = m_selectedActor.transform->Pos() + (m_selectedActor.transform->Left() * delta.x) + (m_selectedActor.transform->Up() * delta.y) + (m_selectedActor.transform->Back() * delta.z);
			//		collider->Pos() = nextPos;
			//		collider->Rot() = m_selectedActor.transform->Rot();
			//	}
			//}

			m_selectedActor.transform->GUIRender();
			for (UINT i = 0; i < m_selectedActor.actor->GetColliders()[m_selectedActor.index].size(); i++)
			{
				m_selectedActor.actor->GetColliders()[m_selectedActor.index][i]->GUIRender();
				DeleteCollider(m_selectedActor.actor, m_selectedActor.actor->GetColliders()[m_selectedActor.index], i);
			}
			AddCollider(m_selectedActor.actor, m_selectedActor.index);
			DeleteActor(m_selectedActor.actor, m_selectedActor.index);
		}

		ImGui::TreePop();
	}

	if (actors.size() > 0)
	{
		label = "Actors";
		if (ImGui::TreeNode(label.c_str()))
		{
			// player start
			if (ImGui::TreeNode("Player Start"))
			{
				SAVELOAD->GetPlayer()->GUIRender();

				ImGui::TreePop();
			}

			for (auto& actor : actors)
			{
				string fbxName = actor->GetName();
				if (ImGui::TreeNode(fbxName.c_str()))
				{
					for (UINT i = 0; i < actor->GetModels()->GetTransforms().size(); i++)
					{
						string actorName = actor->GetModels()->GetTransforms()[i]->GetTag() + "_" + to_string(i + 1);
						if (ImGui::TreeNode(actorName.c_str()))
						{
							actor->GetModels()->GetTransforms()[i]->GUIRender();
							for (UINT j = 0; j < actor->GetColliders()[i].size(); j++)
							{
								actor->GetColliders()[i][j]->GUIRender();
								DeleteCollider(actor, actor->GetColliders()[i], j);
							}

							AddCollider(actor, i);
							DeleteActor(actor, i);

							ImGui::TreePop();
						}
					}

					ImGui::TreePop();
				}

				if (actor->GetModels()->GetTransforms().size() <= 0)
				{
					SAFE_DELETE(actor);
					actors.erase(remove(actors.begin(), actors.end(), actor), actors.end());
					continue;
				}
			}

			ImGui::TreePop();
		}
	}

	AddActor();

	SAVELOAD->GUIRender();

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.8f, 0.8f));
	bool ret = ImGui::Button(u8"���θ޴���", ImVec2(100, 20));
	ImGui::PopStyleColor(3);

	if (ret)
	{
		SCENEMANAGER->ChangeScene("MainMenu");
	}
}

void Scene_MapEditor::SelectActor()
{
	Ray ray = CAMERA->ScreenPointToRay(mousePos);
	Contact contact;
	vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
	for (const auto& actor : actors)
	{
		for (UINT i = 0; i < actor->GetModels()->GetTransforms().size(); i++)
		{
			if (m_selectedActor.transform != actor->GetModels()->GetTransforms()[i])
			{
				for (const auto& collider : actor->GetColliders()[i])
					collider->SetColor(Float4(0, 1, 0, 1));
			}

			for (const auto& collider : actor->GetColliders()[i])
			{
				if (collider->IsRayCollision(ray, &contact) && KEY_DOWN(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
				{
					m_selectedActor.actor = actor;
					m_selectedActor.transform = actor->GetModels()->GetTransforms()[i];
					m_selectedActor.index = i;
					for (const auto& collider : m_selectedActor.actor->GetColliders()[m_selectedActor.index])
						collider->SetColor(Float4(1, 0, 0, 1));
					return;
				}
			}

			for(const auto& collider : actor->GetColliders()[i])
			{
				if (!collider->IsRayCollision(ray, &contact) && KEY_DOWN(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
					m_selectedActor = StoreActor();
			}
		}
	}
}

void Scene_MapEditor::AddActor()
{
	string label = "Actor List";
	bool ret[static_cast<int>(GameActor::GameActorTag::MAX)] = { false };
	vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
	if (ImGui::TreeNode(label.c_str()))
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));

		for (const auto& list : m_fbxList)
		{
			if (ImGui::TreeNode(list.first.c_str()))
			{
				for (const auto& name : list.second)
				{
					bool ret = ImGui::Button(name.c_str(), ImVec2(200, 25));
					bool existence = false;
					InstancingActor* addedActor = nullptr;

					if (ret)
					{
						for (auto& actor : actors)
						{
							// �ش� fbx�� ������ InstancingActor�� �ִ���
							if (!strcmp(actor->GetName().c_str(), name.c_str()))
							{
								existence = true;
								actor->Add();
								addedActor = actor;
								break;
							}
						}

						// �ش� fbx�� ������ InstancingActor�� ���ٸ�(���� for������ return ���� ���ߴٸ�)
						if (!existence)
						{
							actors.push_back(new InstancingActor(GameActor::GameActorTag::ENTITY, list.first, name));
							actors[actors.size() - 1]->Add();
							addedActor = actors[actors.size() - 1];
						}

						m_selectedActor.actor = addedActor;
						m_selectedActor.index = addedActor->GetModels()->GetTransforms().size() - 1;
						m_selectedActor.transform = addedActor->GetModels()->GetTransforms()[m_selectedActor.index];
					}
				}

				ImGui::TreePop();
			}
		}

		ImGui::PopStyleColor(3);

		ImGui::TreePop();
	}
}

void Scene_MapEditor::AddCollider(InstancingActor* actor, UINT index)
{
	string label = actor->GetName() + " Add Collider";
	if (ImGui::TreeNode(label.c_str()))
	{
		const char* colliderList[] = { "Box", "Sphere", "Capsule" };
		ImGui::Combo("ColliderType", (int*)&m_colliderType, colliderList, 3);

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));
		bool ret = ImGui::Button(u8"Collider �߰�", ImVec2(100, 20));
		ImGui::PopStyleColor(3);

		if (ret)
		{
			actor->AddCollider(index, static_cast<Collider::Type>(m_colliderType));
		}

		ImGui::TreePop();
	}
}

void Scene_MapEditor::DeleteActor(InstancingActor* actor, UINT index)
{
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.8f, 0.8f));
	bool ret = ImGui::Button(u8"Actor ����", ImVec2(100, 20));

	if (ret)
	{
		actor->Erase(index);

		if (m_selectedActor.actor == actor)
			m_selectedActor = StoreActor();
	}

	ImGui::PopStyleColor(3);
}

void Scene_MapEditor::DeleteCollider(InstancingActor* actor, vector<Collider*>& colliders, UINT index)
{
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.8f, 0.8f));

	string label = u8"Collider_" + to_string(index) + u8" ����";
	bool ret = ImGui::Button(label.c_str(), ImVec2(150, 20));

	if (ret)
	{
		actor->EraseCollider(colliders, index);
	}

	ImGui::PopStyleColor(3);
}
