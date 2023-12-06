#include "framework.h"
#include "Scene_MapEditor.h"

Scene_MapEditor::Scene_MapEditor()
{
	m_sceneName = "MapEditor";

	vector<string> fileList;
	StringPath::GetFiles(fileList, "Assets/FBX/", "*.fbx", false);
	for (auto& list : fileList)
	{
		// Assets/FBX ������ �ִ� fbx������ Ȯ���� ���� ��� ��������
		list = StringPath::GetFileNameWithoutExtension(list);
		size_t result = list.find('_');

		// ���� �̸��� '_'�� ���ԵǾ� �ִٸ�
		if (result != string::npos)
		{
			string name = list.substr(0, result);

			// ������(���ڸ� �ٸ�) fbx�� �̹� ��ϵǾ� �ִ� ���
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
		else // ���� �̸��� '_'�� ���ٸ�
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
							// �ش� fbx�� ������ InstancingActor�� �ִ���
							if (!strcmp(actor->GetName().c_str(), fbxName.c_str()))
							{
								existence = true;
								actor->Add();
								break;
							}
						}

						// �ش� fbx�� ������ InstancingActor�� ���ٸ�(���� for������ return ���� ���ߴٸ�)
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
	bool ret = ImGui::Button(u8"����", ImVec2(100, 20));
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
