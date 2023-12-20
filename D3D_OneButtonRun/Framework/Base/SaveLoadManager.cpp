#include "SaveLoadManager.h"
#include "framework.h"

SaveLoadManager::SaveLoadManager()
{
	m_player = new GamePlayer();
	m_target = new TargetActor();
	m_mainCamera = new Camera();
	m_lightBuffer = new LightBuffer();
	m_sky = new Sky(L"Textures/CubeMap/Spring.dds");

	LoadRecord();

	WCHAR lpCurrentDirectory[1024];
	GetCurrentDirectory(1024, lpCurrentDirectory);
	m_projectPath = lpCurrentDirectory;
}

SaveLoadManager::~SaveLoadManager()
{
	for (InstancingActor* actor : m_intancingActors)
		SAFE_DELETE(actor);
	m_intancingActors.erase(m_intancingActors.begin(), m_intancingActors.end());

	SAFE_DELETE(m_lightBuffer);
	SAFE_DELETE(m_mainCamera);
	SAFE_DELETE(m_player);
	SAFE_DELETE(m_target);
}

/// <summary>
/// Save���� ImGUI ���� ����
/// </summary>
void SaveLoadManager::Save()
{
	// ImGui ��ư Set
	FileDialog dialog;
	wstring saveFile;
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f/7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f/7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f/7.0f, 0.8f, 0.8f));
	bool ret = ImGui::Button(u8"����", ImVec2(100, 20));
	ImGui::PopStyleColor(3);

	if (!ret) return;

	if (dialog.Save(saveFile, FileDialog::SCENE))
	{
		saveFile = saveFile.substr(m_projectPath.size() + 1, saveFile.size());
		SaveScene(saveFile);	// Saved\\Test
	}
}

void SaveLoadManager::SaveCamera()
{
	bool ret1;

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2.0f / 7.0f, 0.8f, 0.8f));
	ret1 = ImGui::Button(u8"�÷���ȭ�� ����", ImVec2(100, 20));
	ImGui::PopStyleColor(3);

	if (ret1)
	{
		SetPlayCameraTransform(m_mainCamera->Pos(), m_mainCamera->Rot());
	}

	bool ret2;

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3.0f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3.0f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3.0f / 7.0f, 0.8f, 0.8f));
	ImGui::SameLine();
	ret2 = ImGui::Button(u8"Ŭ����ȭ�� ����", ImVec2(100, 20));
	ImGui::PopStyleColor(3);

	if (ret2)
	{
		SetClearCameraTransform(m_mainCamera->Pos(), m_mainCamera->Rot());
	}
}

/// <summary>
/// Scene���� ����: ���ϰ�� ����, Scene�̸�, Camera, Light, Scene�� ���ӿ��� ����
/// </summary>
/// <param name="savePath">Dialog�� ���� ���õ� ���</param>
void SaveLoadManager::SaveScene(wstring savePath)
{
	string saveFile = StringPath::ToString(savePath);
	StringPath::Replace(saveFile, "\\", "/");
	m_savePath = saveFile;
	string fileName = StringPath::GetFileName(saveFile);
	string xml = m_savePath + ".xml";

	// ���丮 ����
	StringPath::CreateFolders(m_savePath);

	// Scene���� ��Ʈ
	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	tinyxml2::XMLElement* scene = document->NewElement(fileName.c_str());
	scene->SetAttribute("Scene", fileName.c_str());
	document->InsertFirstChild(scene);
	
	// ť��� ����
	tinyxml2::XMLElement* cubeMap = scene->InsertNewChildElement("CubeMap");
	cubeMap->SetAttribute("File", StringPath::ToString(m_sky->GetCurrentTextureFile()).c_str());
	scene->InsertEndChild(cubeMap);

	// Camera ���� 
	tinyxml2::XMLElement* camera = scene->InsertNewChildElement("Camera");
	scene->InsertFirstChild(camera);
	// Pos
	tinyxml2::XMLElement* pos = camera->InsertNewChildElement("Position");
	camera->InsertFirstChild(pos);
	pos->SetAttribute("X", m_PlayCameraPos.x);
	pos->SetAttribute("Y", m_PlayCameraPos.y);
	pos->SetAttribute("Z", m_PlayCameraPos.z);
	camera->InsertEndChild(pos);
	// Rot
	tinyxml2::XMLElement* rot = camera->InsertNewChildElement("Rotation");
	camera->InsertFirstChild(rot);
	rot->SetAttribute("X", m_PlayCameraRot.x);
	rot->SetAttribute("Y", m_PlayCameraRot.y);
	rot->SetAttribute("Z", m_PlayCameraRot.z);
	camera->InsertEndChild(rot);
	scene->InsertEndChild(camera);

	// Clearȭ�� Camera Transform
	tinyxml2::XMLElement* clearCamera = scene->InsertNewChildElement("GameClearCameraView");
	scene->InsertFirstChild(clearCamera);
	// Pos
	pos = clearCamera->InsertNewChildElement("Position");
	clearCamera->InsertFirstChild(pos);
	pos->SetAttribute("X", m_clearCameraPos.x);
	pos->SetAttribute("Y", m_clearCameraPos.y);
	pos->SetAttribute("Z", m_clearCameraPos.z);
	clearCamera->InsertEndChild(pos);
	// Rot
	rot = clearCamera->InsertNewChildElement("Rotation");
	clearCamera->InsertFirstChild(rot);
	rot->SetAttribute("X", m_clearCameraRot.x);
	rot->SetAttribute("Y", m_clearCameraRot.y);
	rot->SetAttribute("Z", m_clearCameraRot.z);
	clearCamera->InsertEndChild(rot);
	scene->InsertEndChild(clearCamera);

	// ����Ʈ ����
	tinyxml2::XMLElement* lightInfo = scene->InsertNewChildElement("LightInfo");
	scene->InsertFirstChild(lightInfo);
	// ����Ʈ Color
	tinyxml2::XMLElement* light = lightInfo->InsertNewChildElement("LightColor");
	lightInfo->InsertFirstChild(light);
	light->SetAttribute("R", m_lightBuffer->Get().lights[0].color.x);
	light->SetAttribute("G", m_lightBuffer->Get().lights[0].color.y);
	light->SetAttribute("B", m_lightBuffer->Get().lights[0].color.z);
	light->SetAttribute("A", m_lightBuffer->Get().lights[0].color.w);
	lightInfo->InsertEndChild(light);
	// ����Ʈ Dir
	tinyxml2::XMLElement* dir = lightInfo->InsertNewChildElement("LightDirection");
	lightInfo->InsertFirstChild(dir);
	dir->SetAttribute("X", m_lightBuffer->Get().lights[0].direction.x);
	dir->SetAttribute("Y", m_lightBuffer->Get().lights[0].direction.y);
	dir->SetAttribute("Z", m_lightBuffer->Get().lights[0].direction.z);
	lightInfo->InsertEndChild(dir);
	// �����Ʈ Color
	light = lightInfo->InsertNewChildElement("AmbientLight");
	lightInfo->InsertFirstChild(light);
	light->SetAttribute("R", m_lightBuffer->Get().ambientLight.x);
	light->SetAttribute("G", m_lightBuffer->Get().ambientLight.y);
	light->SetAttribute("B", m_lightBuffer->Get().ambientLight.z);
	lightInfo->InsertEndChild(light);
	// �����Ʈ Ceil
	light = lightInfo->InsertNewChildElement("AmbientCeil");
	lightInfo->InsertFirstChild(light);
	light->SetAttribute("R", m_lightBuffer->Get().ambientCeil.x);
	light->SetAttribute("G", m_lightBuffer->Get().ambientCeil.y);
	light->SetAttribute("B", m_lightBuffer->Get().ambientCeil.z);
	lightInfo->InsertEndChild(light);
	scene->InsertEndChild(lightInfo);

	// GamePlayer ����
	tinyxml2::XMLElement* player = scene->InsertNewChildElement("Player");
	// Pos
	pos = player->InsertNewChildElement("Position");
	player->InsertFirstChild(pos);
	pos->SetAttribute("X", m_player->GetColliderPosition().x);
	pos->SetAttribute("Y", m_player->GetColliderPosition().y);
	pos->SetAttribute("Z", m_player->GetColliderPosition().z);
	player->InsertEndChild(pos);
	// Rot
	rot = player->InsertNewChildElement("Rotation");
	player->InsertFirstChild(rot);
	rot->SetAttribute("X", m_player->GetColliderRotation().x);
	rot->SetAttribute("Y", m_player->GetColliderRotation().y);
	rot->SetAttribute("Z", m_player->GetColliderRotation().z);
	player->InsertEndChild(rot);

	// Target ����
	tinyxml2::XMLElement* target = scene->InsertNewChildElement("Target");
	// Pos
	pos = target->InsertNewChildElement("Position");
	target->InsertFirstChild(pos);
	pos->SetAttribute("X", m_target->GetColliderPosition().x);
	pos->SetAttribute("Y", m_target->GetColliderPosition().y);
	pos->SetAttribute("Z", m_target->GetColliderPosition().z);
	target->InsertEndChild(pos);
	// Rot
	rot = target->InsertNewChildElement("Rotation");
	target->InsertFirstChild(rot);
	rot->SetAttribute("X", m_target->GetColliderRotation().x);
	rot->SetAttribute("Y", m_target->GetColliderRotation().y);
	rot->SetAttribute("Z", m_target->GetColliderRotation().z);
	target->InsertEndChild(rot);

	// GameActor Type�� ����Ʈ 
	tinyxml2::XMLElement* list = scene->InsertNewChildElement("GameActors");
	list->SetAttribute("NumberOfInstances", m_intancingActors.size());
	scene->InsertEndChild(list);

	for (InstancingActor* gameActor : m_intancingActors)
	{
		tinyxml2::XMLElement* list_type = list->InsertNewChildElement("ActorInfo");
		list_type->SetAttribute("Count", gameActor->GetModels()->GetTransforms().size());
		list_type->SetAttribute("FBXFolder", gameActor->GetFolderName().c_str());
		list_type->SetAttribute("FBX", gameActor->GetName().c_str());

		int count = gameActor->GetModels()->GetTransforms().size();
		SaveActor(gameActor, count, list_type);
	}

	document->SaveFile(xml.c_str());
	SAFE_DELETE(document);
}

/// <summary>
/// Scene�� Player�� ���͵� �ν��Ͻ������� ���� ����
/// </summary>
/// <param name="gameActor">�ν��Ͻ� ���� ���ӿ���</param>
/// <param name="count">�ν��Ͻ̵� ���� FBX ���� ����</param>
/// <param name="list">���� XML���</param>
void SaveLoadManager::SaveActor(InstancingActor* gameActor, int count, tinyxml2::XMLElement* list)
{
	for (int i = 0; i < count; ++i)
	{
		string actIndex = to_string(i);
		actIndex = "Actor_" + actIndex;

		// ��Ʈ
		tinyxml2::XMLElement* model = list->InsertNewChildElement(actIndex.c_str());

		// Ȱ��ȭ ����
		tinyxml2::XMLElement* modelActive = model->InsertNewChildElement("ModelActive");
		modelActive->SetAttribute("isActive", gameActor->GetModels()->GetTransforms()[i]->IsActive());
		model->InsertFirstChild(modelActive);

		// �� Ʈ������ ����
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

		// �ݸ��� Ʈ������ ����
		tinyxml2::XMLElement* colliders = model->InsertNewChildElement("Colliders");
		colliders->SetAttribute("NumberOfColliders", gameActor->GetColliders()[i].size());

		for (int j = 0; j < gameActor->GetColliders()[i].size(); ++j)
		{
			string colIndex = to_string(j);
			colIndex = "Collider_" + colIndex;

			tinyxml2::XMLElement* collider = colliders->InsertNewChildElement(colIndex.c_str());
			collider->SetAttribute("Type", gameActor->GetColliders()[i][j]->GetType());
			// Pos
			pos = collider->InsertNewChildElement("Position");
			collider->InsertFirstChild(pos);
			pos->SetAttribute("X", gameActor->GetColliders()[i][j]->Pos().x);
			pos->SetAttribute("Y", gameActor->GetColliders()[i][j]->Pos().y);
			pos->SetAttribute("Z", gameActor->GetColliders()[i][j]->Pos().z);
			collider->InsertEndChild(pos);
			// Rot
			rot = collider->InsertNewChildElement("Rotation");
			collider->InsertFirstChild(rot);
			rot->SetAttribute("X", gameActor->GetColliders()[i][j]->Rot().x);
			rot->SetAttribute("Y", gameActor->GetColliders()[i][j]->Rot().y);
			rot->SetAttribute("Z", gameActor->GetColliders()[i][j]->Rot().z);
			collider->InsertEndChild(rot);
			// Scale
			scale = collider->InsertNewChildElement("Scale");
			collider->InsertFirstChild(scale);
			scale->SetAttribute("X", gameActor->GetColliders()[i][j]->Scale().x);
			scale->SetAttribute("Y", gameActor->GetColliders()[i][j]->Scale().y);
			scale->SetAttribute("Z", gameActor->GetColliders()[i][j]->Scale().z);
			collider->InsertEndChild(scale);
		}
	}
}

void SaveLoadManager::SaveRecord(UINT stage, UINT count, float time)
{
	// ���� ��ϰ� ��
	if (m_records.size() > stage)
	{
		// ��ư�� �켱
		//if (m_records[stage].count > count)
		//{
		//	m_records[stage].count = count;
		//	m_records[stage].time = time;
		//}

		// �ð� �켱
		if (m_records[stage].time > time)
		{
			m_records[stage].time = time;
			m_records[stage].count = count;
		}
	}
	else
	{
		Record newRecord;
		newRecord.count = count;
		newRecord.time = time;

		m_records.push_back(newRecord);
	}

	// binary ����
	string path = "Saved/Record/PlayRecord.bin";

	StringPath::CreateFolders(path);
	BinaryWriter* writer = new BinaryWriter(path);

	// ũ������
	writer->UInt(m_records.size());

	// ��ϵ� ����
	for (Record record : m_records)
	{
		writer->UInt(record.count);
		writer->Float(record.time);
	}

	SAFE_DELETE(writer);
}

/// <summary>
/// Load���� ImGUI ���� ����
/// </summary>
void SaveLoadManager::Load()
{
	// ImGui ��ư Set
	FileDialog dialog;
	wstring saveFile;
	bool ret;

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4.0f/7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4.0f/7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4.0f/7.0f, 0.8f, 0.8f));
	ret = ImGui::Button(u8"�ε�", ImVec2(100, 20));
	ImGui::PopStyleColor(3);

	if (!ret) return;

	if (dialog.Open(saveFile, FileDialog::SCENE))
	{
		saveFile = saveFile.substr(m_projectPath.size() + 1, saveFile.size());
		LoadScene(saveFile);
	}
}

/// <summary>
/// Scene���� �ε�: ���ϰ�� ����, Scene�̸�, Camera, Light, Scene�� ���ӿ��� ����, �� ���ӿ��� ����
/// </summary>
/// <param name="savePath">Dialog�� ���� ���õ� ���</param>
void SaveLoadManager::LoadScene(wstring savePath)
{
	// ��� ���Ӿ��� �ʱ�ȭ
	m_intancingActors.erase(m_intancingActors.begin(), m_intancingActors.end());
	m_intancingActors.clear();

	string saveFile = StringPath::ToString(savePath);
	StringPath::Replace(saveFile, "\\", "/");

	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	document->LoadFile(saveFile.c_str());

	// Scene���� ��Ʈ
	tinyxml2::XMLElement* scene = document->FirstChildElement();
	string sceneName = scene->Attribute("Scene");

	// CubeMap ����
	tinyxml2::XMLElement* cubeMap = scene->FirstChildElement();
	m_sky->SetTexture(StringPath::ToWString(cubeMap->Attribute("File")));

	// Camera ���� 
	tinyxml2::XMLElement* camera = cubeMap->NextSiblingElement();
	// Pos
	tinyxml2::XMLElement* pos = camera->FirstChildElement();
	m_mainCamera->Pos() = Vector3(pos->FloatAttribute("X"), pos->FloatAttribute("Y"), pos->FloatAttribute("Z"));
	// Rot
	tinyxml2::XMLElement* rot = pos->NextSiblingElement();
	m_mainCamera->Rot() = Vector3(rot->FloatAttribute("X"), rot->FloatAttribute("Y"), rot->FloatAttribute("Z"));

	// Camera Clear View ���� 
	tinyxml2::XMLElement* clearCamera = camera->NextSiblingElement();
	// Pos
	pos = clearCamera->FirstChildElement();
	m_clearCameraPos = Vector3(pos->FloatAttribute("X"), pos->FloatAttribute("Y"), pos->FloatAttribute("Z"));
	// Rot
	rot = pos->NextSiblingElement();
	m_clearCameraRot = Vector3(rot->FloatAttribute("X"), rot->FloatAttribute("Y"), rot->FloatAttribute("Z"));

	// ����Ʈ ����
	tinyxml2::XMLElement* lightInfo = clearCamera->NextSiblingElement();
	// ����Ʈ Color
	tinyxml2::XMLElement* light = lightInfo->FirstChildElement();
	m_lightBuffer->Get().lights[0].color = Float4(light->FloatAttribute("R"), light->FloatAttribute("G"), light->FloatAttribute("B"), light->FloatAttribute("A"));
	// ����Ʈ Dir
	tinyxml2::XMLElement* dir = light->NextSiblingElement();
	m_lightBuffer->Get().lights[0].direction = Vector3(dir->FloatAttribute("X"), dir->FloatAttribute("Y"), dir->FloatAttribute("Z"));
	// �����Ʈ Color
	tinyxml2::XMLElement* ambientLight = dir->NextSiblingElement();
	m_lightBuffer->Get().ambientLight = Float3(ambientLight->FloatAttribute("R"), ambientLight->FloatAttribute("G"), ambientLight->FloatAttribute("B"));
	// �����Ʈ Ceil
	tinyxml2::XMLElement* ambientCeil = ambientLight->NextSiblingElement();
	m_lightBuffer->Get().ambientCeil = Float3(ambientLight->FloatAttribute("R"), ambientLight->FloatAttribute("G"), ambientLight->FloatAttribute("B"));

	// Player ����
	tinyxml2::XMLElement* player = lightInfo->NextSiblingElement();
	pos = player->FirstChildElement();
	m_player->SetPosition(Vector3(pos->FloatAttribute("X"), pos->FloatAttribute("Y"), pos->FloatAttribute("Z")));
	m_playerOriginPos = Vector3(pos->FloatAttribute("X"), pos->FloatAttribute("Y"), pos->FloatAttribute("Z"));
	rot = pos->NextSiblingElement();
	m_player->SetRotation(Vector3(rot->FloatAttribute("X"), rot->FloatAttribute("Y"), rot->FloatAttribute("Z")));
	m_playerOriginRot = Vector3(rot->FloatAttribute("X"), rot->FloatAttribute("Y"), rot->FloatAttribute("Z"));
	
	// target ����
	tinyxml2::XMLElement* target = player->NextSiblingElement();
	pos = target->FirstChildElement();
	m_target->SetPosition(Vector3(pos->FloatAttribute("X"), pos->FloatAttribute("Y"), pos->FloatAttribute("Z")));
	rot = pos->NextSiblingElement();
	m_target->SetRotation(Vector3(rot->FloatAttribute("X"), rot->FloatAttribute("Y"), rot->FloatAttribute("Z")));

	// GameActor ���� 
	tinyxml2::XMLElement* gameActors = target->NextSiblingElement();
	int size = stoi(gameActors->Attribute("NumberOfInstances"));
	tinyxml2::XMLElement* actorInfo = gameActors->FirstChildElement();

	// Game Actor ����
	for (int i = 0; i < size; ++i)
	{
		int size = actorInfo->IntAttribute("Count");
		string folderName = actorInfo->Attribute("FBXFolder");
		string fbxName = actorInfo->Attribute("FBX");
		InstancingActor* instActor = new InstancingActor(GameActor::ENTITY, folderName, fbxName);
		tinyxml2::XMLElement* index = actorInfo->FirstChildElement();

		for (int j = 0; j < size; ++j)
		{
			int id;
			id = instActor->Add();
			// Ȱ��ȭ ����
			tinyxml2::XMLElement* modelActive = index->FirstChildElement();
			instActor->GetModels()->GetTransforms()[j]->SetActive(modelActive->Attribute("isActive"));
			// �� Ʈ������ ����
			tinyxml2::XMLElement* transform = modelActive->NextSiblingElement();
			// Pos
			pos = transform->FirstChildElement();
			instActor->GetModels()->GetTransforms()[j]->Pos() = Vector3(pos->FloatAttribute("X"), pos->FloatAttribute("Y"), pos->FloatAttribute("Z"));
			// Pos
			rot = pos->NextSiblingElement();
			instActor->GetModels()->GetTransforms()[j]->Rot() = Vector3(rot->FloatAttribute("X"), rot->FloatAttribute("Y"), rot->FloatAttribute("Z"));
			// Scale
			tinyxml2::XMLElement* scale = rot->NextSiblingElement();
			instActor->GetModels()->GetTransforms()[j]->Scale() = Vector3(scale->FloatAttribute("X"), scale->FloatAttribute("Y"), scale->FloatAttribute("Z"));
			// �ݸ��� Ʈ������ ����
			tinyxml2::XMLElement* colliders = transform->NextSiblingElement();
			int count = 0;
			count = colliders->IntAttribute("NumberOfColliders");
			tinyxml2::XMLElement* collider = colliders->FirstChildElement();
			Collider::Type colliderType;
			for (int k = 0; k < count; ++k)
			{
				colliderType = static_cast<Collider::Type>(stoi(collider->Attribute("Type")));
				instActor->AddCollider(j, colliderType);
				// Pos
				pos = collider->FirstChildElement();
				instActor->GetColliders()[j][k]->Pos() = Vector3(pos->FloatAttribute("X"), pos->FloatAttribute("Y"), pos->FloatAttribute("Z"));
				// Pos
				rot = pos->NextSiblingElement();
				instActor->GetColliders()[j][k]->Rot() = Vector3(rot->FloatAttribute("X"), rot->FloatAttribute("Y"), rot->FloatAttribute("Z"));
				// Scale
				scale = rot->NextSiblingElement();
				instActor->GetColliders()[j][k]->Scale() = Vector3(scale->FloatAttribute("X"), scale->FloatAttribute("Y"), scale->FloatAttribute("Z"));

				tinyxml2::XMLElement* tTemp = collider->NextSiblingElement();
				collider = tTemp;
			}

			tinyxml2::XMLElement* temp = index->NextSiblingElement();
			index = temp;
		}
		m_intancingActors.push_back(instActor);

		tinyxml2::XMLElement* temp = actorInfo->NextSiblingElement();
		actorInfo = temp;
	}

	SAFE_DELETE(document);
}

void SaveLoadManager::LoadRecord()
{
	BinaryReader reader("Saved/Record/PlayRecord.bin");
	if (reader.IsFailed())
		return;

	// Records ũ��
	UINT size = reader.UInt();
	m_records.reserve(size);

	for (UINT i = 0; i < size; i++)
	{
		Record record;
		record.count = reader.UInt();
		record.time = reader.Float();

		m_records.push_back(record);
	}

	printf("");
}

/// <summary>
/// Scene ȯ�� �ʱ�ȭ �� ��� ���ӿ��� ����
/// </summary>
void SaveLoadManager::Clear()
{
	// ImGui ��ư Set
	bool ret;

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f / 7.0f, 0.8f, 0.8f));
	ret = ImGui::Button(u8"�ʱ�ȭ", ImVec2(100, 20));
	ImGui::PopStyleColor(3);

	if (ret)
	{
		// ī�޶� �ʱ�ȭ
		m_mainCamera->InitCamera();
		// ����Ʈ �ʱ�ȭ
		m_lightBuffer->Init();
		// �÷��̾� �ʱ�ȭ
		m_player->InitTransform();
		// ���Ӿ��� �ʱ�ȭ
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