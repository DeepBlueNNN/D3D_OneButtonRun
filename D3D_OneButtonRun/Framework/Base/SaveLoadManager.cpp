#include "SaveLoadManager.h"
#include "framework.h"

// 1) 플레이기록 별도 저장
// 2) SAVELOAD->SetLoadScene (테스트필요)
// 3) SAVELOAD->LoadScene (테스트필요)

SaveLoadManager::SaveLoadManager()
{
	m_player = new GamePlayer();
	m_mainCamera = new Camera();
	m_lightBuffer = new LightBuffer();
	m_sky = new Sky(L"Textures/CubeMap/Spring.dds");

	WCHAR lpCurrentDirectory[1024];
	GetCurrentDirectory(1024, lpCurrentDirectory);
	m_projectPath = lpCurrentDirectory;
}

SaveLoadManager::~SaveLoadManager()
{
	m_intancingActors.erase(m_intancingActors.begin(), m_intancingActors.end());
	m_intancingActors.clear();

	SAFE_DELETE(m_lightBuffer);
	SAFE_DELETE(m_mainCamera);
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

	if (dialog.Save(saveFile, FileDialog::SCENE))
	{
		saveFile = saveFile.substr(m_projectPath.size() + 1, saveFile.size());
		SaveScene(saveFile);	// Saved\\Test
	}
}

void SaveLoadManager::SaveScene(wstring savePath)
{
	string saveFile = StringPath::ToString(savePath);
	StringPath::Replace(saveFile, "\\", "/");
	m_savePath = saveFile;
	string fileName = StringPath::GetFileName(saveFile);
	string xml = m_savePath + ".xml";

	// 디렉토리 생성
	StringPath::CreateFolders(m_savePath);

	// Scene정보 루트
	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	tinyxml2::XMLElement* scene = document->NewElement(fileName.c_str());
	scene->SetAttribute("Scene", fileName.c_str());
	document->InsertFirstChild(scene);
	
	// 큐브맵 정보
	tinyxml2::XMLElement* cubeMap = scene->InsertNewChildElement("CubeMap");
	cubeMap->SetAttribute("File", StringPath::ToString(m_sky->GetCurrentTextureFile()).c_str());
	scene->InsertEndChild(cubeMap);

	// Camera 정보 
	tinyxml2::XMLElement* camera = scene->InsertNewChildElement("Camera");
	scene->InsertFirstChild(camera);
	// Pos
	tinyxml2::XMLElement* pos = camera->InsertNewChildElement("Position");
	camera->InsertFirstChild(pos);
	pos->SetAttribute("X", m_mainCamera->Pos().x);
	pos->SetAttribute("Y", m_mainCamera->Pos().y);
	pos->SetAttribute("Z", m_mainCamera->Pos().z);
	camera->InsertEndChild(pos);
	// Rot
	tinyxml2::XMLElement* rot = camera->InsertNewChildElement("Rotation");
	camera->InsertFirstChild(rot);
	rot->SetAttribute("X", m_mainCamera->Rot().x);
	rot->SetAttribute("Y", m_mainCamera->Rot().y);
	rot->SetAttribute("Z", m_mainCamera->Rot().z);
	camera->InsertEndChild(rot);
	scene->InsertEndChild(camera);

	// 라이트 정보
	tinyxml2::XMLElement* lightInfo = scene->InsertNewChildElement("LightInfo");
	scene->InsertFirstChild(lightInfo);
	// 라이트 Color
	tinyxml2::XMLElement* light = lightInfo->InsertNewChildElement("LightColor");
	lightInfo->InsertFirstChild(light);
	light->SetAttribute("R", m_lightBuffer->Get().lights[0].color.x);
	light->SetAttribute("G", m_lightBuffer->Get().lights[0].color.y);
	light->SetAttribute("B", m_lightBuffer->Get().lights[0].color.z);
	light->SetAttribute("A", m_lightBuffer->Get().lights[0].color.w);
	lightInfo->InsertEndChild(light);
	// 라이트 Dir
	tinyxml2::XMLElement* dir = lightInfo->InsertNewChildElement("LightDirection");
	lightInfo->InsertFirstChild(dir);
	dir->SetAttribute("X", m_lightBuffer->Get().lights[0].direction.x);
	dir->SetAttribute("Y", m_lightBuffer->Get().lights[0].direction.y);
	dir->SetAttribute("Z", m_lightBuffer->Get().lights[0].direction.z);
	lightInfo->InsertEndChild(dir);
	// 엠비언트 Color
	light = lightInfo->InsertNewChildElement("AmbientLight");
	lightInfo->InsertFirstChild(light);
	light->SetAttribute("R", m_lightBuffer->Get().ambientLight.x);
	light->SetAttribute("G", m_lightBuffer->Get().ambientLight.y);
	light->SetAttribute("B", m_lightBuffer->Get().ambientLight.z);
	lightInfo->InsertEndChild(light);
	// 엠비언트 Ceil
	light = lightInfo->InsertNewChildElement("AmbientCeil");
	lightInfo->InsertFirstChild(light);
	light->SetAttribute("R", m_lightBuffer->Get().ambientCeil.x);
	light->SetAttribute("G", m_lightBuffer->Get().ambientCeil.y);
	light->SetAttribute("B", m_lightBuffer->Get().ambientCeil.z);
	lightInfo->InsertEndChild(light);
	scene->InsertEndChild(lightInfo);

	// GamePlayer 저장
	tinyxml2::XMLElement* player = scene->InsertNewChildElement("Player");
	// Pos
	pos = player->InsertNewChildElement("Position");
	player->InsertFirstChild(pos);
	pos->SetAttribute("X", m_player->GetModelPosition().x);
	pos->SetAttribute("Y", m_player->GetModelPosition().y);
	pos->SetAttribute("Z", m_player->GetModelPosition().z);
	player->InsertEndChild(pos);
	// Rot
	rot = player->InsertNewChildElement("Rotation");
	player->InsertFirstChild(rot);
	rot->SetAttribute("X", m_player->GetModelRotation().x);
	rot->SetAttribute("Y", m_player->GetModelRotation().y);
	rot->SetAttribute("Z", m_player->GetModelRotation().z);
	player->InsertEndChild(rot);
	// Scale
	tinyxml2::XMLElement* scale = player->InsertNewChildElement("Scale");
	player->InsertFirstChild(scale);
	scale->SetAttribute("X", m_player->GetModelScale().x);
	scale->SetAttribute("Y", m_player->GetModelScale().y);
	scale->SetAttribute("Z", m_player->GetModelScale().z);
	player->InsertEndChild(scale);

	// GameActor Type별 리스트 
	tinyxml2::XMLElement* list = scene->InsertNewChildElement("GameActors");
	list->SetAttribute("NumberOfInstances", m_intancingActors.size());
	scene->InsertEndChild(list);

	for (InstancingActor* gameActor : m_intancingActors)
	{
		GameActor::GameActorTag type = gameActor->GetGameActorType();

		for (int i = 1; i < GameActor::MAX; ++i)
		{
			if (type == i)
			{
				tinyxml2::XMLElement* list_type = list->InsertNewChildElement("ActorInfo");
				list_type->SetAttribute("Count", gameActor->GetColliders().size());
				list_type->SetAttribute("Type", to_string(i).c_str());
				list_type->SetAttribute("FBX", gameActor->GetName().c_str());
				//list_type->SetAttribute("ColliderType", gameActor->GetColliderType());

				int count = gameActor->GetColliders().size();
				SaveActor(gameActor, type, count, list_type);
			}
		}
	}

	document->SaveFile(xml.c_str());
	SAFE_DELETE(document);
}

void SaveLoadManager::SaveActor(InstancingActor* gameActor, GameActor::GameActorTag actorType, int count, tinyxml2::XMLElement* list)
{
	for (int i = 0; i < count; ++i)
	{
		string index = to_string(i);
		index = "InstanceActor_" + index;
		// 루트
		tinyxml2::XMLElement* model = list->InsertNewChildElement(index.c_str());
		list->InsertFirstChild(model);

		// 활성화 여부
		tinyxml2::XMLElement* modelActive = model->InsertNewChildElement("ModelActive");
		modelActive->SetAttribute("isActive", gameActor->GetModels()->GetTransforms()[i]->IsActive());
		model->InsertFirstChild(modelActive);

		// 모델 트렌스폼 정보
		tinyxml2::XMLElement* transform = model->InsertNewChildElement("ModelTransform");
		// Pos
		tinyxml2::XMLElement* pos = transform->InsertNewChildElement("Position");
		transform->InsertFirstChild(pos);
		pos->SetAttribute("X", gameActor->GetModels()->GetTransforms()[i]->Pos().x);
		pos->SetAttribute("Y", gameActor->GetModels()->GetTransforms()[i]->Pos().y);
		pos->SetAttribute("Z", gameActor->GetModels()->GetTransforms()[i]->Pos().z);
		transform->InsertEndChild(pos);
		// Rot
		tinyxml2::XMLElement* rot = transform->InsertNewChildElement("Rotation");
		transform->InsertFirstChild(rot);
		rot->SetAttribute("X", gameActor->GetModels()->GetTransforms()[i]->Rot().x);
		rot->SetAttribute("Y", gameActor->GetModels()->GetTransforms()[i]->Rot().y);
		rot->SetAttribute("Z", gameActor->GetModels()->GetTransforms()[i]->Rot().z);
		transform->InsertEndChild(rot);
		// Scale
		tinyxml2::XMLElement* scale = transform->InsertNewChildElement("Scale");
		transform->InsertFirstChild(scale);
		scale->SetAttribute("X", gameActor->GetModels()->GetTransforms()[i]->Scale().x);
		scale->SetAttribute("Y", gameActor->GetModels()->GetTransforms()[i]->Scale().y);
		scale->SetAttribute("Z", gameActor->GetModels()->GetTransforms()[i]->Scale().z);
		transform->InsertEndChild(scale);

		list->InsertEndChild(model);

		//// 콜리더 트렌스폼 정보
		//tinyxml2::XMLElement* transform2 = model->InsertNewChildElement("ColliderTransform");
		//// Pos
		//pos = transform2->InsertNewChildElement("Position");
		//transform2->InsertFirstChild(pos);
		//pos->SetAttribute("X", gameActor->GetColliders()[i]->Pos().x);
		//pos->SetAttribute("Y", gameActor->GetColliders()[i]->Pos().y);
		//pos->SetAttribute("Z", gameActor->GetColliders()[i]->Pos().z);
		//transform2->InsertEndChild(pos);
		//// Rot
		//rot = transform2->InsertNewChildElement("Rotation");
		//transform2->InsertFirstChild(rot);
		//rot->SetAttribute("X", gameActor->GetColliders()[i]->Rot().x);
		//rot->SetAttribute("Y", gameActor->GetColliders()[i]->Rot().y);
		//rot->SetAttribute("Z", gameActor->GetColliders()[i]->Rot().z);
		//transform2->InsertEndChild(rot);
		//// Scale
		//scale = transform2->InsertNewChildElement("Scale");
		//transform2->InsertFirstChild(scale);
		//scale->SetAttribute("X", gameActor->GetColliders()[i]->Scale().x);
		//scale->SetAttribute("Y", gameActor->GetColliders()[i]->Scale().y);
		//scale->SetAttribute("Z", gameActor->GetColliders()[i]->Scale().z);
		//transform2->InsertEndChild(scale);
	}
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

	if (dialog.Open(saveFile, FileDialog::SCENE))
	{
		saveFile = saveFile.substr(m_projectPath.size() + 1, saveFile.size());
		LoadScene(saveFile);
	}
}

void SaveLoadManager::LoadScene(wstring savePath)
{
	// 멤버 게임액터 초기화
	m_intancingActors.erase(m_intancingActors.begin(), m_intancingActors.end());
	m_intancingActors.clear();

	string saveFile = StringPath::ToString(savePath);
	StringPath::Replace(saveFile, "\\", "/");

	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	document->LoadFile(saveFile.c_str());

	// Scene정보 루트
	tinyxml2::XMLElement* scene = document->FirstChildElement();
	string sceneName = scene->Attribute("Scene");

	// CubeMap 정보
	tinyxml2::XMLElement* cubeMap = scene->FirstChildElement();
	m_sky->SetTexture(StringPath::ToWString(cubeMap->Attribute("File")));
	// Camera 정보 
	tinyxml2::XMLElement* camera = cubeMap->NextSiblingElement();
	// Pos
	tinyxml2::XMLElement* pos = camera->FirstChildElement();
	m_mainCamera->Pos() = Vector3(pos->FloatAttribute("X"), pos->FloatAttribute("Y"), pos->FloatAttribute("Z"));
	// Rot
	tinyxml2::XMLElement* rot = pos->NextSiblingElement();
	m_mainCamera->Rot() = Vector3(rot->FloatAttribute("X"), rot->FloatAttribute("Y"), rot->FloatAttribute("Z"));

	// 라이트 정보
	tinyxml2::XMLElement* lightInfo = camera->NextSiblingElement();
	// 라이트 Color
	tinyxml2::XMLElement* light = lightInfo->FirstChildElement();
	m_lightBuffer->Get().lights[0].color = Float4(light->FloatAttribute("R"), light->FloatAttribute("G"), light->FloatAttribute("B"), light->FloatAttribute("A"));
	// 라이트 Dir
	tinyxml2::XMLElement* dir = light->NextSiblingElement();
	m_lightBuffer->Get().lights[0].direction = Vector3(dir->FloatAttribute("X"), dir->FloatAttribute("Y"), dir->FloatAttribute("Z"));
	// 엠비언트 Color
	tinyxml2::XMLElement* ambientLight = dir->NextSiblingElement();
	m_lightBuffer->Get().ambientLight = Float3(ambientLight->FloatAttribute("R"), ambientLight->FloatAttribute("G"), ambientLight->FloatAttribute("B"));
	// 엠비언트 Ceil
	tinyxml2::XMLElement* ambientCeil = ambientLight->NextSiblingElement();
	m_lightBuffer->Get().ambientCeil = Float3(ambientLight->FloatAttribute("R"), ambientLight->FloatAttribute("G"), ambientLight->FloatAttribute("B"));

	// Player 정보
	tinyxml2::XMLElement* player = lightInfo->NextSiblingElement();
	pos = player->FirstChildElement();
	m_player->SetPosition(Vector3(pos->FloatAttribute("X"), pos->FloatAttribute("Y"), pos->FloatAttribute("Z")));
	rot = pos->NextSiblingElement();
	m_player->SetRotation(Vector3(rot->FloatAttribute("X"), rot->FloatAttribute("Y"), rot->FloatAttribute("Z")));
	tinyxml2::XMLElement* scale = rot->NextSiblingElement();
	m_player->SetModelScale(Vector3(scale->FloatAttribute("X"), scale->FloatAttribute("Y"), scale->FloatAttribute("Z")));

	// GameActor 갯수 
	tinyxml2::XMLElement* gameActors = player->NextSiblingElement();
	int size = stoi(gameActors->Attribute("NumberOfInstances"));
	tinyxml2::XMLElement* actorInfo = gameActors->FirstChildElement();

	// Game Actor 루프
	for (int i = 0; i < size; ++i)
	{
		int size = actorInfo->IntAttribute("Count");
		GameActor::GameActorTag actorType = static_cast<GameActor::GameActorTag>(stoi(actorInfo->Attribute("Type")));
		string fbxName = actorInfo->Attribute("FBX");
		Collider::Type colliderType = static_cast<Collider::Type>(stoi(actorInfo->Attribute("ColliderType")));
		InstancingActor* instActor = new InstancingActor(actorType, fbxName);
		tinyxml2::XMLElement* index = actorInfo->FirstChildElement();

		for (int j = 0; j < size; ++j)
		{
			int id;
			id = instActor->Add();
			// 활성화 여부
			tinyxml2::XMLElement* modelActive = index->FirstChildElement();
			instActor->GetModels()->GetTransforms()[id]->SetActive(modelActive->Attribute("isActive"));
			// 모델 트렌스폼 정보
			tinyxml2::XMLElement* transform = modelActive->NextSiblingElement();
			// Pos
			pos = transform->FirstChildElement();
			instActor->GetModels()->GetTransforms()[id]->Pos() = Vector3(pos->FloatAttribute("X"), pos->FloatAttribute("Y"), pos->FloatAttribute("Z"));
			// Pos
			rot = pos->NextSiblingElement();
			instActor->GetModels()->GetTransforms()[id]->Rot() = Vector3(rot->FloatAttribute("X"), rot->FloatAttribute("Y"), rot->FloatAttribute("Z"));
			// Scale
			scale = rot->NextSiblingElement();
			instActor->GetModels()->GetTransforms()[id]->Scale() = Vector3(scale->FloatAttribute("X"), scale->FloatAttribute("Y"), scale->FloatAttribute("Z"));
			// 콜리더 트렌스폼 정보
			tinyxml2::XMLElement* transform2 = transform->NextSiblingElement();
			//// Pos
			//pos = transform2->FirstChildElement();
			//instActor->GetColliders()[id]->Pos() = Vector3(pos->FloatAttribute("X"), pos->FloatAttribute("Y"), pos->FloatAttribute("Z"));
			//// Pos
			//rot = pos->NextSiblingElement();
			//instActor->GetColliders()[id]->Rot() = Vector3(rot->FloatAttribute("X"), rot->FloatAttribute("Y"), rot->FloatAttribute("Z"));
			//// Scale
			//scale = rot->NextSiblingElement();
			//instActor->GetColliders()[id]->Scale() = Vector3(scale->FloatAttribute("X"), scale->FloatAttribute("Y"), scale->FloatAttribute("Z"));

			tinyxml2::XMLElement* temp = index->NextSiblingElement();
			index = temp;
		}
		m_intancingActors.push_back(instActor);

		tinyxml2::XMLElement* temp = actorInfo->NextSiblingElement();
		actorInfo = temp;
	}

	SAFE_DELETE(document);
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
		// 카메라 초기화
		m_mainCamera->InitCamera();
		// 라이트 초기화
		m_lightBuffer->Init();
		// 플레이어 초기화
		m_player->InitTransform();
		// 게임액터 초기화
		m_intancingActors.erase(m_intancingActors.begin(), m_intancingActors.end());
		m_intancingActors.clear();
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

GameActor* SaveLoadManager::CreateInstancingActor(GameActor::GameActorTag type, string fbxName)
{
	GameActor* actor = nullptr;

	//switch (type)
	//{
	//case GameActor::PLAYER:
	//	actor = new GamePlayer();
	//	break;

	//case GameActor::TREE:
	//	actor = new Tree(fbxName);
	//	break;

	//case GameActor::ROCK:
	//	actor = new Rock(fbxName);
	//	break;
	//}

	return actor;
}