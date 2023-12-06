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
			auto find = fbxList.find(name);
			if (find != fbxList.end())
			{
				find->second++;
			}
			else
			{
				fbxList.insert(make_pair(name, 1));
			}
		}
		else // 파일 이름에 '_'가 없다면
		{
			fbxList.insert(make_pair(list, 1));
		}
	}
}

Scene_MapEditor::~Scene_MapEditor()
{
}

void Scene_MapEditor::Update()
{
	vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
	for (const auto actor : actors)
		actor->Update();

	SelectActor();
}

void Scene_MapEditor::Render()
{
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
	vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();

	//// SelectedActor's GUIRender
	string label = "Selected Actor";
	if (ImGui::TreeNode(label.c_str()))
	{
		if (m_selectedActor.transform)
		{
			m_selectedActor.transform->GUIRender();
			m_selectedActor.actor->GetColliders()[m_selectedActor.index]->GUIRender();
			DeleteActor();
		}

		ImGui::TreePop();
	}

	label = "Actors";
	if (ImGui::TreeNode(label.c_str()))
	{
		for (const auto actor : actors)
			actor->GUIRender();

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
	for (auto& actor : actors)
	{
		for (int i = 0; i < actor->GetColliders().size(); i++)
		{
			if (m_selectedActor.transform != actor->GetModels()->GetTransforms()[i])
				actor->GetColliders()[i]->SetColor(Float4(0, 1, 0, 1));

			if (actor->GetColliders()[i]->IsRayCollision(ray, &contact) && KEY_PRESS(VK_LBUTTON))
			{
				m_selectedActor.actor = actor;
				m_selectedActor.transform = actor->GetModels()->GetTransforms()[i];
				m_selectedActor.index = i;
				actor->GetColliders()[i]->SetColor(Float4(1, 0, 0, 1));
				return;
			}

			if (!actor->GetColliders()[i]->IsRayCollision(ray, &contact) && KEY_PRESS(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
				m_selectedActor = StoreActor();
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
		const char* colliderList[] = { "Box", "Sphere", "Capsule" };
		ImGui::Combo("ColliderType", (int*)&m_colliderType, colliderList, 3);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));

		for (const auto& list : fbxList)
		{
			if (ImGui::TreeNode(list.first.c_str()))
			{
				for (int i = 1; i < list.second + 1; i++)
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
							actors.push_back(new InstancingActor(GameActor::GameActorTag::ENTITY, fbxName,
								static_cast<Collider::Type>(m_colliderType)));
							actors[actors.size() - 1]->Add();
						}
					}
				}

				ImGui::TreePop();
			}
		}

		ImGui::PopStyleColor(3);

		ImGui::TreePop();
	}
}

void Scene_MapEditor::DeleteActor()
{
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.8f, 0.8f));
	bool ret = ImGui::Button(u8"삭제", ImVec2(100, 20));
	ImGui::PopStyleColor(3);

	if (ret)
	{
		m_selectedActor.actor->Erase(m_selectedActor.index);
		if (!m_selectedActor.actor->GetColliders().size())
		{
			vector<InstancingActor*>& actors = SAVELOAD->GetInstancingActors();
			for (int i = 0; i < actors.size(); i++)
			{
				if (actors[i] == m_selectedActor.actor)
				{
					actors.erase(actors.begin() + i);
					break;
				}
			}
		}
		m_selectedActor = StoreActor();
	}
}
