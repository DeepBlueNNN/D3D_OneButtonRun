#include "framework.h"
#include "Scene/S00_TestScene.h"
#include "Scene/S01_CubeMap.h"
#include "Scene/S02_Grid.h"

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
	m_scenes.push_back(new S01_CubeMap());
	m_scenes.push_back(new S02_Grid());
	m_scenes.push_back(new S00_TestScene());
}

SceneManager::~SceneManager()
{
	for (UINT i = 0; i < m_scenes.size(); i++)
		delete m_scenes[i];

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
	for (UINT i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i]->IsActive())
			m_scenes[i]->Render();
	}
}

void SceneManager::PostRender()
{
	Font::Get()->GetDC()->BeginDraw();
	int  temp = (int)(ImGui::GetIO().Framerate);

	string fps = "FPS : " + to_string(temp);
	Font::Get()->RenderText(fps, { 100.0f, (float)(MAIN->GetHeight() - 10) });

	for (UINT i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i]->IsActive())
			m_scenes[i]->PostRender();
	}

	Font::Get()->GetDC()->EndDraw();
}

void SceneManager::GuiRender()
{
	ENV->GUIRender();

	for (UINT i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i]->IsActive())
			m_scenes[i]->GUIRender();
	}
}

void SceneManager::ChangeScene(string name)
{
	for (UINT i = 0; i < m_scenes.size(); i++)
	{
		m_scenes[i]->SetActive(false);

		if (name == m_scenes[i]->GetName())
		{
			m_scenes[i]->SetActive(true);
			m_scenes[i]->ChangeScene();
		}
	}

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
