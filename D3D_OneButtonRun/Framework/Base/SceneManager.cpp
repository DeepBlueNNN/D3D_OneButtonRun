#include "framework.h"
#include "Scene/S00_TestScene.h"
#include "Scene/S01_CubeMap.h"
#include "Scene/S02_Grid.h"
#include "Scene/Scene_MainMenu.h"
#include "Scene/Scene_MapEditor.h"
#include "Scene/Scene_GamePlay.h"

/*
	#include "Scene/S00_[SCENE CLASS NAME].h"
	생성 Scene 헤더 추가
*/

SceneManager::SceneManager()
{
	ENV;

	// 스레드를 이용하여 여러개의 Scene을 Load
	/*
		m_scenes.pushback(new [SCENE CLASS NAME]());
		활성화 할 Scene vector컨테이너에 pushback 
	*/
	m_scenes.push_back(new S01_CubeMap());
//	m_scenes.push_back(new S02_Grid());
//	m_scenes.push_back(new S00_TestScene());
	m_scenes.push_back(new Scene_GamePlay());
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
		if (m_scenes[i]->IsActive())   // Scene 활성화되어있다면
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
// HW : 스레드와 프로세스의 차이점
// 프로세스는 독립된 영역으로 Code,Data,Stack,Heap
//           구조를 할당
//          Code  : program coding
//          Data  : 변수
//          Stack :
//          Heap  : 동적할당
// 스레드는 현재 프로세스 내에서 공유(Stack X)
// 비용문제: 프로세스 <-> 프로세스  : IPC
//           Inter Process communication
///////////////////////////////////////////////
void SceneManager::ThreadStart()
{
	long  start = GetTickCount64();
	long  end = GetTickCount64();

	m_isThreadEnd = true;

	printf("스레드 End  : %d\n", end - start);
}
