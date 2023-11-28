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

private:
	vector<GameActor*> m_gameActors;
	wstring m_projectPath = L"";
	string m_savePath = "";
};
