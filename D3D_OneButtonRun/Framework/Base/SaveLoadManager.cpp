#include "SaveLoadManager.h"
#include "framework.h"

SaveLoadManager::SaveLoadManager()
{
	WCHAR lpCurrentDirectory[1024];
	GetCurrentDirectory(1024, lpCurrentDirectory);
	m_projectPath = lpCurrentDirectory;
}

SaveLoadManager::~SaveLoadManager()
{
	m_gameActors.erase(m_gameActors.begin(), m_gameActors.end());
	m_gameActors.clear();
}

void SaveLoadManager::Save()
{
	// ImGui 버튼 Set
	FileDialog dialog;
	wstring saveFile;
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f/7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f/7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f/7.0f, 0.8f, 0.8f));
	bool ret = ImGui::Button(u8"저장", ImVec2(100, 20));
	ImGui::PopStyleColor(3);

	if (!ret) return;

	if (dialog.Save(saveFile, 2))
	{
		saveFile = saveFile.substr(m_projectPath.size() + 1, saveFile.size());
		SaveScene(saveFile);	// Saved\\Test
	}
}

void SaveLoadManager::SaveScene(wstring savePath)
{
	string saveFile = StringPath::ToString(savePath);
	StringPath::Replace(saveFile, "\\", "/");
	string fileName = StringPath::GetFileName(saveFile);
	m_savePath = saveFile + "/";
	string xml = m_savePath + fileName + ".xml";

	// 디렉토리 생성
	StringPath::CreateFolders(m_savePath);

	// Scene정보 루트
	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	tinyxml2::XMLElement* scene = document->NewElement(fileName.c_str());
	scene->SetAttribute("Scene", fileName.c_str());
	document->InsertFirstChild(scene);
	
	// GameActor Type별 리스트 
	tinyxml2::XMLElement* list = scene->InsertNewChildElement("GameActors");
	list->SetAttribute("NumActors", m_gameActors.size());
	scene->InsertFirstChild(list);

	for (GameActor* gameActor : m_gameActors)
	{
		SaveActor(gameActor, list);
	}

	document->SaveFile(xml.c_str());
	SAFE_DELETE(document);
}

void SaveLoadManager::SaveActor(GameActor* gameActor, tinyxml2::XMLElement* list)
{
	// 루트
	tinyxml2::XMLElement* model = list->InsertNewChildElement("GameActor");
	model->SetAttribute("Type", gameActor->GetGameActorType());
	model->SetAttribute("InGameName", gameActor->GetName().c_str());
	model->SetAttribute("FBXName", gameActor->GetModel()->GetName().c_str());
	list->InsertFirstChild(model);

	// 활성화 여부
	tinyxml2::XMLElement* modelActive = model->InsertNewChildElement("ModelActive");
	modelActive->SetAttribute("isActive", gameActor->GetActive());
	model->InsertFirstChild(modelActive);

	// 모델 트렌스폼 정보
	tinyxml2::XMLElement* transform = model->InsertNewChildElement("ModelTransform");
	// Pos
	tinyxml2::XMLElement* pos = transform->InsertNewChildElement("Position");
	transform->InsertFirstChild(pos);
	pos->SetAttribute("X", gameActor->GetModelPosition().x);
	pos->SetAttribute("Y", gameActor->GetModelPosition().y);
	pos->SetAttribute("Z", gameActor->GetModelPosition().z);
	transform->InsertEndChild(pos);
	// Rot
	tinyxml2::XMLElement* rot = transform->InsertNewChildElement("Rotation");
	transform->InsertFirstChild(rot);
	rot->SetAttribute("X", gameActor->GetModelRotation().x);
	rot->SetAttribute("Y", gameActor->GetModelRotation().y);
	rot->SetAttribute("Z", gameActor->GetModelRotation().z);
	transform->InsertEndChild(rot);
	// Scale
	tinyxml2::XMLElement* scale = transform->InsertNewChildElement("Scale");
	transform->InsertFirstChild(scale);
	scale->SetAttribute("X", gameActor->GetModelScale().x);
	scale->SetAttribute("Y", gameActor->GetModelScale().y);
	scale->SetAttribute("Z", gameActor->GetModelScale().z);
	transform->InsertEndChild(scale);

	// 콜리더 트렌스폼 정보
	tinyxml2::XMLElement* transform2 = model->InsertNewChildElement("ColliderTransform");
	/*
	// Pos
	pos = transform2->InsertNewChildElement("Position");
	transform2->InsertFirstChild(pos);
	pos->SetAttribute("X", gameActor->GetColliderPosition().x);
	pos->SetAttribute("Y", gameActor->GetColliderPosition().y);
	pos->SetAttribute("Z", gameActor->GetColliderPosition().z);
	transform2->InsertEndChild(pos);
	// Rot
	rot = transform2->InsertNewChildElement("Rotation");
	transform2->InsertFirstChild(rot);
	rot->SetAttribute("X", gameActor->GetColliderRotation().x);
	rot->SetAttribute("Y", gameActor->GetColliderRotation().y);
	rot->SetAttribute("Z", gameActor->GetColliderRotation().z);
	transform2->InsertEndChild(rot);
	*/
	// Scale
	scale = transform2->InsertNewChildElement("Scale");
	transform2->InsertFirstChild(scale);
	scale->SetAttribute("X", gameActor->GetColliderScale().x);
	scale->SetAttribute("Y", gameActor->GetColliderScale().y);
	scale->SetAttribute("Z", gameActor->GetColliderScale().z);
	transform2->InsertEndChild(scale);
}

void SaveLoadManager::Load()
{
	// ImGui 버튼 Set
	FileDialog dialog;
	wstring saveFile;
	bool ret;

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4.0f/7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4.0f/7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4.0f/7.0f, 0.8f, 0.8f));
	ret = ImGui::Button(u8"로드", ImVec2(100, 20));
	ImGui::PopStyleColor(3);

	if (!ret) return;

	if (dialog.Open(saveFile))
	{
		saveFile = saveFile.substr(m_projectPath.size() + 1, saveFile.size());
		LoadScene(saveFile);
	}
}

void SaveLoadManager::LoadScene(wstring savePath)
{
	// 멤버 게임액터 초기화
	m_gameActors.erase(m_gameActors.begin(), m_gameActors.end());
	m_gameActors.clear();

	string saveFile = StringPath::ToString(savePath);
	StringPath::Replace(saveFile, "\\", "/");

	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	document->LoadFile(saveFile.c_str());

	// Scene정보 루트
	tinyxml2::XMLElement* scene = document->FirstChildElement();
	string sceneName = scene->Attribute("Scene");

	// GameActor 갯수 
	tinyxml2::XMLElement* list = scene->FirstChildElement();
	int size = stoi(list->Attribute("NumActors"));

	// Game Actor 루프
	tinyxml2::XMLElement* model = list->FirstChildElement();
	for (int i = 0; i < size; ++i)
	{
		string id = model->Attribute("InGameName");
		size_t index = id.find_last_of('_');
		id = id.substr(index + 1, id.length());
		string fbxName = model->Attribute("FBXName");

		GameActor* actor = nullptr;

		// Type별 클래스 정의
		int type = stoi(model->Attribute("Type"));
		switch (type)
		{
		case 0:
			actor = new GamePlayer();
			break;
		case 1:
			actor = new Tree(fbxName, id);
			break;
		case 2:
			actor = new Rock(fbxName, id);
			break;
		}

		actor->Update();

		// 활성화 여부
		tinyxml2::XMLElement* modelActive = model->FirstChildElement();
		actor->SetActive(modelActive->Attribute("isActive"));
		// 모델 트렌스폼 정보
		tinyxml2::XMLElement* transform = modelActive->NextSiblingElement();
		// Pos
		tinyxml2::XMLElement* pos = transform->FirstChildElement();
		actor->SetPosition(Vector3(pos->FloatAttribute("X"), pos->FloatAttribute("Y"), pos->FloatAttribute("Z")));
		// Rot
		tinyxml2::XMLElement* rot = pos->NextSiblingElement();
		actor->SetRotation(Vector3(rot->FloatAttribute("X"), rot->FloatAttribute("Y"), rot->FloatAttribute("Z")));
		// Scale
		tinyxml2::XMLElement* scale = rot->NextSiblingElement();
		actor->SetModelScale(Vector3(scale->FloatAttribute("X"), scale->FloatAttribute("Y"), scale->FloatAttribute("Z")));

		// 콜리더 트렌스폼 정보
		tinyxml2::XMLElement* transform2 = transform->NextSiblingElement();
		// Scale
		scale = transform2->FirstChildElement();
		actor->SetColliderScale(Vector3(scale->FloatAttribute("X"), scale->FloatAttribute("Y"), scale->FloatAttribute("Z")));
		m_gameActors.push_back(actor);

		tinyxml2::XMLElement* temp = model;
		model = temp->NextSiblingElement();
	}

	SAFE_DELETE(document);
}

void SaveLoadManager::LoadInstancedGameActor()
{
	//m_modelInstanced = new ModelInstancing("Jin");
	//m_modelInstanced->Scale() = Vector3(0.05f, 0.05f, 0.05f);
	//
	//for (int z = 0; z < 10; z++)
	//{
	//	for (int x = 0; x < 10; x++)
	//	{
	//		Transform* transform = m_modelInstanced->Add();
	//		transform->Scale() = Vector3(0.05f, 0.05f, 0.05f);
	//		transform->Pos() = { (float)x * 10.0f, 0.0f, (float)z * 10.0f };
	//	}
	//}
}


void SaveLoadManager::Clear()
{
	// ImGui 버튼 Set
	bool ret;

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.8f, 0.8f));
	ret = ImGui::Button(u8"초기화", ImVec2(100, 20));
	ImGui::PopStyleColor(3);

	if (ret)
	{
		// 멤버 게임액터 초기화
		m_gameActors.erase(m_gameActors.begin(), m_gameActors.end());
		m_gameActors.clear();
	}
}

void SaveLoadManager::GUIRender()
{
	Save();
	ImGui::SameLine();
	Load();
	ImGui::SameLine();
	Clear();
}