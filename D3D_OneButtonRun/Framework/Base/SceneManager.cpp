#include "framework.h"
#include "Scene/S00_TestScene.h"
#include "Scene/S01_CubeMap.h"
#include "Scene/S02_Grid.h"
#include "Scene/Scene_MainMenu.h"
#include "Scene/Scene_SelectStage.h"
#include "Scene/Scene_MapEditor.h"
#include "Scene/Scene_GamePlay.h"

/*
	#include "Scene/S00_[SCENE CLASS NAME].h"
	���� Scene ��� �߰�
*/

SceneManager::SceneManager()
{
	ENV;

	// �����带 �̿��Ͽ� �������� Scene�� Load
	/*
		m_scenes.pushback(new [SCENE CLASS NAME]());
		Ȱ��ȭ �� Scene vector�����̳ʿ� pushback 
	*/
//	m_scenes.push_back(new S01_CubeMap());
//	m_scenes.push_back(new S02_Grid());
//	m_scenes.push_back(new S00_TestScene());

	Scene* scene = new Scene_GamePlay();
	scene->SetActive(false);
	m_scenes.push_back(scene);

	scene = new Scene_MapEditor();
	scene->SetActive(false);
	m_scenes.push_back(scene);

	scene = new Scene_SelectStage();
	scene->SetActive(false);
	m_scenes.push_back(scene);

	scene = new S02_Grid();
	scene->SetActive(false);
	m_scenes.push_back(scene);

	m_scenes.push_back(new Scene_MainMenu());

	m_subMenu = new SubMenu();
}

SceneManager::~SceneManager()
{
	for (UINT i = 0; i < m_scenes.size(); i++)
		SAFE_DELETE(m_scenes[i]);

	m_scenes.erase(m_scenes.begin(), m_scenes.end());
}

void SceneManager::Update()
{
	m_currentScene = nullptr;

	ENV->Set();
	CAMERA->Update();
	ENV->Update();

	for (UINT i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i]->IsActive())   // Scene Ȱ��ȭ�Ǿ��ִٸ�
		{
			m_currentScene = m_scenes[i];
			m_scenes[i]->Update();
		}
	}
}

void SceneManager::Render()
{
	SAVELOAD->GetSky()->Render();

	for (UINT i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i]->IsActive())
			m_scenes[i]->Render();
	}
}

void SceneManager::PostRender()
{
	//Font::Get()->GetDC()->BeginDraw();
	//int  temp = (int)(ImGui::GetIO().Framerate);

	//string fps = "FPS : " + to_string(temp);
	//Font::Get()->RenderText(fps, { 100.0f, (float)(MAIN->GetHeight() - 10) });

	for (UINT i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i]->IsActive())
			m_scenes[i]->PostRender();
	}
	//Font::Get()->GetDC()->EndDraw();
}

void SceneManager::GuiRender()
{
	string fps = "FPS : " + to_string((int)ImGui::GetIO().Framerate);
	ImGui::Text(fps.c_str());
	ImGui::Separator();

	//ENV->GUIRender();

	for (UINT i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i]->IsActive())
			m_scenes[i]->GUIRender();
	}
}

void SceneManager::ChangeScene(string name, bool withGrid)
{
	for (UINT i = 0; i < m_scenes.size(); i++)
	{
		// �ʿ����Ϳ����� �׸���Scene Ȱ��ȭ
		if (withGrid && m_scenes[i]->GetName() == "Grid")
			m_scenes[i]->SetActive(true);
		else
			m_scenes[i]->SetActive(false);

		if (name == m_scenes[i]->GetName())
		{
			m_scenes[i]->SetActive(true);
			m_scenes[i]->ChangeScene();
		}
	}

	// Ȱ��ȭ�� Scene�� �ʷε�
	if (SAVELOAD->GetLoadPath() != L"")
		SAVELOAD->LoadScene(SAVELOAD->GetLoadPath());

	m_strSceneName = name;
}

Scene* SceneManager::GetScene(string name)
{
	Scene* value = nullptr;

	for (UINT i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i]->GetName() != name)
			continue;

		value = m_scenes[i];
		break;
	}

	return value;
}

///////////////////////////////////////////////
// HW : ������� ���μ����� ������
// ���μ����� ������ �������� Code,Data,Stack,Heap
//           ������ �Ҵ�
//          Code  : program coding
//          Data  : ����
//          Stack :
//          Heap  : �����Ҵ�
// ������� ���� ���μ��� ������ ����(Stack X)
// ��빮��: ���μ��� <-> ���μ���  : IPC
//           Inter Process communication
///////////////////////////////////////////////
void SceneManager::ThreadStart()
{
	long  start = GetTickCount64();
	long  end = GetTickCount64();

	m_isThreadEnd = true;

	printf("������ End  : %d\n", end - start);
}
