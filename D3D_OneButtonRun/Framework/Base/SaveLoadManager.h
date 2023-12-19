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
	void SaveActor(InstancingActor* gameActor, int count, tinyxml2::XMLElement* type);

public:
	void Load();
	void LoadScene(wstring savePath);
	/// <summary>
	/// 로드할 파일 경로 Set
	/// </summary>
	/// <param name="savePath">저장된 경로</param>
	void SetLoadFile(wstring savePath) { m_loadPath = savePath; }

public:
	void Clear();

public:
	void GUIRender();

public:
	GamePlayer*& GetPlayer() { return m_player; }
	TargetActor*& GetTargetActor() { return m_target; }
	/// <summary>
	/// 각 Scene에서 사용할 인스턴싱된 GameActor들 레퍼런스 Get 
	/// </summary>
	/// <returns>초기화 또는 로드된 InstancedGameActor들</returns>
	vector<InstancingActor*>& GetInstancingActors() { return m_intancingActors; }
	/// <summary>
	/// 배경 Sky 가져오기
	/// </summary>
	/// <returns>m_sky</returns>
	Sky*& GetSky() { return m_sky; }
	/// <summary>
	/// 메인카메라 받아오기
	/// </summary>
	/// <returns>m_mainCamera</returns>
	Camera*& GetMainCamera() { return m_mainCamera; }
	/// <summary>
	/// Environment에서 사용될 LightBuffer 받아오기
	/// </summary>
	/// <returns>m_lightBuffer</returns>
	LightBuffer*& GetLightBuffer() { return m_lightBuffer; }
	vector<string>& GetClassNames() { return m_classNames; }
	vector<string>& GetFBXNames() { return m_fbxNames; }
	/// <summary>
	/// 저장된 플레이 기록 받아오기
	/// </summary>
	/// <returns>m_record</returns>
	vector<string> GetRecord() { return m_record; }
	wstring GetLoadPath() { return m_loadPath; }

private:
	// GameActors
	GamePlayer* m_player;
	TargetActor* m_target;
	vector<InstancingActor*> m_intancingActors;
	vector<string> m_classNames;
	vector<string> m_fbxNames;
	Sky* m_sky = nullptr;
	
	// SavePath
	wstring m_projectPath = L"";
	string m_savePath = "";

	// LoadPath
	wstring m_loadPath = L"";

	// Record
	vector<string> m_record;

	// Environment
	Camera* m_mainCamera = nullptr;
	LightBuffer* m_lightBuffer = nullptr;
};
