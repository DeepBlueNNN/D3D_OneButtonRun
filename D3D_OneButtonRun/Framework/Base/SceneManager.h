#pragma once

/////////////////////////////////////////////////////////////////
//                   Scene ���� Class						   //
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
	void PostRender();     // 2D��� Rendering
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
	void ThreadStart();  // �������� Scene�� �ִ� ��� ���⼭ ����

private:
	string					m_strSceneName = "";
	vector<class Scene*>	m_scenes;		// �������� Scene�� ��� �ִ� vector �����̳�
	Scene*					m_currentScene = nullptr;
	bool					m_isThreadEnd = false;
	class SubMenu*			m_subMenu = nullptr;
	UINT					m_escCount = 0;
	bool					m_isSubMenuOn = false;
	bool					m_isGUIActive = false;
};

//////////////////////////////////////////////////////////////
// Scene 1-100�� �־� Loading�� �ð� �ɸ��� ���				//
// IntroScene Simple�ϰ� ����� Loading�Ǵ� ���				//
// ������ Scene���� ��Ƽ�����带 �̿��Ͽ� ��׶���� �����Ѵ�	//
//////////////////////////////////////////////////////////////