#pragma once

/////////////////////////////////////////////////////////////////
//                   Scene 생성 Class						   //
/////////////////////////////////////////////////////////////////

class SceneManager : public Singleton<SceneManager>
{
public:
	friend class Singleton;

private:
	SceneManager();
	~SceneManager();

public:
	void Update();         // Scene Update
	void Render();         // Scene Rendering
	void PostRender();     // 2D요소 Rendering
	void GuiRender();      // ImGui Rendering

public:	//Getter
	Scene* GetScene(string name);
	Scene* GetCurrentScene()		{ return m_currentScene; }
	
	bool GetGUIActive() { return m_isGUIActive; }

public: //Setter
	void SetSceneActive(string sceneName, bool isActive);
	void SetGUIActive(bool isGUIActive) { m_isGUIActive = isGUIActive; }

public:
	void ChangeScene(string name, bool withGrid = false);
	void AddScene(Scene* scene)	{ m_scenes.push_back(scene); }
	bool IsThredEnd()				{ return m_isThreadEnd; }

private:
	void ThreadStart();  // 여러개의 Scene이 있는 경우 여기서 생성

private:
	string					m_strSceneName = "";
	vector<class Scene*>	m_scenes;		// 여러개의 Scene을 담아 넣는 vector 컨테이너
	Scene*					m_currentScene = nullptr;
	bool					m_isThreadEnd = false;
	class SubMenu*			m_subMenu = nullptr;
	UINT					m_escCount = 0;
	bool					m_isSubMenuOn = false;
	bool					m_isGUIActive = false;
};

//////////////////////////////////////////////////////////////
// Scene 1-100이 있어 Loading시 시간 걸리는 경우				//
// IntroScene Simple하게 만들어 Loading되는 모습				//
// 나머지 Scene들은 멀티스레드를 이용하여 백그라운드로 생성한다	//
//////////////////////////////////////////////////////////////