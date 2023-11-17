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

public:
	void ChangeScene(string name);
	void AddScene(Scene* scene)	{ m_scenes.push_back(scene); }
	bool IsThredEnd()				{ return m_isThreadEnd; }

private:
	void ThreadStart();  // �������� Scene�� �ִ� ��� ���⼭ ����

private:
	string					m_strSceneName = "";
	vector<class Scene*>	m_scenes;		// �������� Scene�� ��� �ִ� vector �����̳�
	Scene*					m_currentScene = nullptr;
	bool					m_isThreadEnd = false;
};

//////////////////////////////////////////////////////////////
// Scene 1-100�� �־� Loading�� �ð� �ɸ��� ���				//
// IntroScene Simple�ϰ� ����� Loading�Ǵ� ���				//
// ������ Scene���� ��Ƽ�����带 �̿��Ͽ� ��׶���� �����Ѵ�	//
//////////////////////////////////////////////////////////////