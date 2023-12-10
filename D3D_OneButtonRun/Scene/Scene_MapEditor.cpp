#include "framework.h"
#include "Scene_MapEditor.h"

Scene_MapEditor::Scene_MapEditor()
{
	m_sceneName = "MapEditor";

	vector<string> fileList;
	StringPath::GetFiles(fileList, "Assets/FBX/", "*.fbx", false);
	for (auto& list : fileList)
	{
		// Assets/FBX 폴더에 있는 fbx파일을 확장자 빼고 모두 가져오기
		list = StringPath::GetFileNameWithoutExtension(list);
		size_t result = list.find('_');

		// 파일 이름에 '_'가 포함되어 있다면
		if (result != string::npos)
		{
			string name = list.substr(0, result);

			// 동일한(숫자만 다른) fbx가 이미 등록되어 있는 경우
			auto find = m_fbxList.find(name);
			if (find != m_fbxList.end())
			{
				find->second++;
			}
			else
			{
				m_fbxList.insert(make_pair(name, 1));
			}
		}
		else // 파일 이름에 '_'가 없다면
		{
			m_fbxList.insert(make_pair(list, 1));
		}
	}
}

Scene_MapEditor::~Scene_MapEditor()
{
}

void Scene_MapEditor::Update()
{
	ENV->Set();
	ENV->Update();

	vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
	for (const auto actor : actors)
		actor->Update();

	SelectActor();
}

void Scene_MapEditor::Render()
{
	SAVELOAD->GetSky()->Render();

	vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
	for (const auto actor : actors)
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
	ENV->GUIRender();
	SAVELOAD->GetSky()->GUIRender();
	vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();

	// SelectedActor's GUIRender
	string label = "Selected Actor";
	if (ImGui::TreeNode(label.c_str()))
	{
		if (m_selectedActor.transform)
		{
			if (ImGui::DragFloat3("Actor Position", (float*)&m_actorPos, 0.1f))
			{
				m_selectedActor.transform->Pos() += m_actorPos;
				for (const auto& collider : m_selectedActor.actor->GetColliders()[m_selectedActor.index])
					collider->Pos() += m_actorPos;
				m_actorPos = Vector3();
			}

			m_selectedActor.transform->GUIRender();
			for (UINT i = 0; i < m_selectedActor.actor->GetColliders()[m_selectedActor.index].size(); i++)
			{
				m_selectedActor.actor->GetColliders()[m_selectedActor.index][i]->GUIRender();
				DeleteCollider(m_selectedActor.actor->GetColliders()[m_selectedActor.index], i);
			}
			AddCollider(m_selectedActor.actor, m_selectedActor.index);
			DeleteActor();
		}

		ImGui::TreePop();
	}

	label = "Actors";
	if (ImGui::TreeNode(label.c_str()))
	{
		for (const auto& actor : actors)
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
							DeleteCollider(actor->GetColliders()[i], j);
						}

						AddCollider(actor, i);
						DeleteActor();

						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}

	AddActor();

	SAVELOAD->GUIRender();
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
				if (collider->IsRayCollision(ray, &contact) && KEY_DOWN(VK_LBUTTON))
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
				for (UINT i = 1; i < list.second + 1; i++)
				{
					string name = list.first + "_" + to_string(i);
					bool ret = ImGui::Button(name.c_str(), ImVec2(200, 25));

					bool existence = false;
					if (ret)
					{
						string fbxName = list.first + "_" + to_string(i);
						for (auto& actor : actors)
						{
							// 해당 fbx로 생성된 InstancingActor가 있는지
							if (!strcmp(actor->GetName().c_str(), fbxName.c_str()))
							{
								existence = true;
								actor->Add();
								break;
							}
						}

						// 해당 fbx로 생성된 InstancingActor가 없다면(위의 for문에서 return 되지 못했다면)
						if (!existence)
						{
							actors.push_back(new InstancingActor(GameActor::GameActorTag::ENTITY, fbxName));
							actors[actors.size() - 1]->Add();
						}

						m_selectedActor.actor = actors[actors.size() - 1];
						m_selectedActor.index = actors[actors.size() - 1]->GetModels()->GetTransforms().size() - 1;
						m_selectedActor.transform = m_selectedActor.actor->GetModels()->
							GetTransforms()[m_selectedActor.actor->GetModels()->GetTransforms().size() - 1];
					}
				}

				ImGui::TreePop();
			}
		}

		ImGui::PopStyleColor(3);

		ImGui::TreePop();
	}
}

void Scene_MapEditor::AddCollider(InstancingActor* actor, int index)
{
	string label = actor->GetName() + " Add Collider";
	if (ImGui::TreeNode(label.c_str()))
	{
		const char* colliderList[] = { "Box", "Sphere", "Capsule" };
		ImGui::Combo("ColliderType", (int*)&m_colliderType, colliderList, 3);

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));
		bool ret = ImGui::Button(u8"Collider 추가", ImVec2(100, 20));
		ImGui::PopStyleColor(3);

		if (ret)
		{
			actor->AddCollider(index, static_cast<Collider::Type>(m_colliderType));
		}

		ImGui::TreePop();
	}
}

void Scene_MapEditor::DeleteActor()
{
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.8f, 0.8f));
	bool ret = ImGui::Button(u8"Actor 삭제", ImVec2(100, 20));
	ImGui::PopStyleColor(3);

	if (ret)
	{
		m_selectedActor.actor->Erase(m_selectedActor.index);
		if (!m_selectedActor.actor->GetColliders().size())
		{
			vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
			for (UINT i = 0; i < actors.size(); i++)
			{
				if (actors[i] == m_selectedActor.actor)
				{
					actors[i]->Erase(i);
					break;
				}
			}
		}
		m_selectedActor = StoreActor();
	}
}

void Scene_MapEditor::DeleteCollider(vector<Collider*>& colliders, int index)
{
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.8f, 0.8f));
	bool ret = ImGui::Button(u8"Collider 삭제", ImVec2(100, 20));

	if (ret)
	{
		SAFE_DELETE(colliders[index]);

		colliders.erase(colliders.begin() + index);
	}

	ImGui::PopStyleColor(3);
}
