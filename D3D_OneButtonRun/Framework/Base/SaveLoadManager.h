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
	/// GameActor Class 추가 후 이 함수 내용에 추가한 Class 추가하기
	/// </summary>
	/// <param name="type">enum GameActor::GameActorTag</param>
	/// <param name="fbxName">파일 이름만(확장자 없이)</param>
	/// <param name="id">Editor상의 중복된 Class 번호</param>
	/// <returns>함수에서 생성된 GameActor를 반환</returns>
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
