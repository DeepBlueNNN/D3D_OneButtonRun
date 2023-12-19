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
	/// �ε��� ���� ��� Set
	/// </summary>
	/// <param name="savePath">����� ���</param>
	void SetLoadFile(wstring savePath) { m_loadPath = savePath; }

public:
	void Clear();

public:
	void GUIRender();

public:
	GamePlayer*& GetPlayer() { return m_player; }
	TargetActor*& GetTargetActor() { return m_target; }
	/// <summary>
	/// �� Scene���� ����� �ν��Ͻ̵� GameActor�� ���۷��� Get 
	/// </summary>
	/// <returns>�ʱ�ȭ �Ǵ� �ε�� InstancedGameActor��</returns>
	vector<InstancingActor*>& GetInstancingActors() { return m_intancingActors; }
	/// <summary>
	/// ��� Sky ��������
	/// </summary>
	/// <returns>m_sky</returns>
	Sky*& GetSky() { return m_sky; }
	/// <summary>
	/// ����ī�޶� �޾ƿ���
	/// </summary>
	/// <returns>m_mainCamera</returns>
	Camera*& GetMainCamera() { return m_mainCamera; }
	/// <summary>
	/// Environment���� ���� LightBuffer �޾ƿ���
	/// </summary>
	/// <returns>m_lightBuffer</returns>
	LightBuffer*& GetLightBuffer() { return m_lightBuffer; }
	vector<string>& GetClassNames() { return m_classNames; }
	vector<string>& GetFBXNames() { return m_fbxNames; }
	/// <summary>
	/// ����� �÷��� ��� �޾ƿ���
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
