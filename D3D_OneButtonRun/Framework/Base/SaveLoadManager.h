#pragma once
class GameActor;

class SaveLoadManager : public Singleton<SaveLoadManager>
{
public:
	friend class Singleton;

public:
	SaveLoadManager();
	~SaveLoadManager();

public:
	void Save();
	void SaveScene(wstring savePath);
	void SaveActor(GameActor* gameActor, tinyxml2::XMLElement* type);

public:
	void Load();
	void LoadScene(wstring savePath);
	void LoadInstancedGameActor();

public:
	void Clear();

public:
	void GUIRender();
	vector<GameActor*>& GetGameActors() { return m_gameActors; }
	/// <summary>
	/// GameActor Class �߰� �� �� �Լ� ���뿡 �߰��� Class �߰��ϱ�
	/// </summary>
	/// <param name="type">enum GameActor::GameActorTag</param>
	/// <param name="fbxName">���� �̸���(Ȯ���� ����)</param>
	/// <param name="id">Editor���� �ߺ��� Class ��ȣ</param>
	/// <returns>�Լ����� ������ GameActor�� ��ȯ</returns>
	GameActor* CreateInstancingActor(GameActor::GameActorTag type, string fbxName);
	vector<string>& GetClassNames() { return m_classNames; }
	vector<string>& GetFBXNames() { return m_fbxNames; }

private:
	vector<GameActor*> m_gameActors;
	wstring m_projectPath = L"";
	string m_savePath = "";
	vector<string> m_classNames;
	vector<string> m_fbxNames;
};
