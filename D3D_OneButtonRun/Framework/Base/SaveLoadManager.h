#pragma once
class GameActor;

struct Record
{
	UINT count = 0;
	float time = 0.0f;
};

class SaveLoadManager : public Singleton<SaveLoadManager>
{
public:
	friend class Singleton;

public:
	SaveLoadManager();
	~SaveLoadManager();

public:
	void Save();
	void SaveCamera();
	void SaveScene(wstring savePath);
	void SaveActor(InstancingActor* gameActor, int count, tinyxml2::XMLElement* type);
	void SaveRecord(UINT stage, UINT count, float time);

public:
	void Load();
	void LoadScene(wstring savePath);
	void LoadRecord();

public:
	void Clear();

public:
	void GUIRender();

public: //Getter
	GamePlayer*& GetPlayer() { return m_player; }
	Vector3 GetPlayerOriginPos() { return m_playerOriginPos; }
	Vector3 GetPlayerOriginRot() { return m_playerOriginRot; }
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
	/// ���� Ŭ���� �� ������ ȭ���� ī�޶� ������ ��������
	/// -�� �ʸ��� ����� ī�޶� ������ load�� ����
	/// </summary>
	/// <returns>m_clearCameraPos</returns>
	Vector3 GetClearCameraPos() { return m_clearCameraPos; }
	/// <summary>
	/// ���� Ŭ���� �� ������ ȭ���� ī�޶� �����̼� ��������
	/// -�� �ʸ��� ����� ī�޶� �����̼� load�� ����
	/// </summary>
	/// <returns>m_clearCameraRot</returns>
	Vector3 GetClearCameraRot() { return m_clearCameraRot; }
	/// <summary>
	/// Environment���� ���� LightBuffer �޾ƿ���
	/// </summary>
	/// <returns>m_lightBuffer</returns>
	LightBuffer*& GetLightBuffer() { return m_lightBuffer; }
	/// <summary>
	/// ����� �÷��� ��� �޾ƿ���
	/// </summary>
	/// <returns>m_record</returns>
	vector<Record> GetRecords() { return m_records; }
	wstring GetLoadPath() { return m_loadPath; }
	UINT GetCurrentStage() { return m_currentStage; }

public: //Setter
	/// <summary>
	/// �ε��� ���� ��� Set
	/// </summary>
	/// <param name="savePath">����� ���</param>
	void SetLoadFile(UINT index, wstring savePath) { m_currentStage = index; m_loadPath = savePath; }
	void SetPlayCameraTransform(Vector3 pos, Vector3 rot) { m_PlayCameraPos = m_mainCamera->Pos();  m_PlayCameraRot = m_mainCamera->Rot(); }
	void SetCameraPlayView() { m_mainCamera->Pos() = m_PlayCameraPos; m_mainCamera->Rot() = m_PlayCameraRot; }
	void SetClearCameraTransform(Vector3 pos, Vector3 rot) { m_clearCameraPos = pos; m_clearCameraRot = rot; }
	void SetCameraClearView() { m_mainCamera->Pos() = m_clearCameraPos; m_mainCamera->Rot() = m_clearCameraRot; }

private:
	// GameActors
	GamePlayer* m_player = nullptr;
	Vector3 m_playerOriginPos = Vector3(0.0f,0.0f,0.0f);
	Vector3 m_playerOriginRot = Vector3(0.0f, 0.0f, 0.0f);
	TargetActor* m_target = nullptr;
	vector<InstancingActor*> m_intancingActors;

	Sky* m_sky = nullptr;
	
	// SavePath
	wstring m_projectPath = L"";
	string m_savePath = "";

	// LoadPath
	UINT m_currentStage = 0;
	wstring m_loadPath = L"";

	// Record
	vector<Record> m_records;

	// Environment
	Camera* m_mainCamera = nullptr;
	Vector3 m_PlayCameraPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_PlayCameraRot = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_clearCameraPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_clearCameraRot = Vector3(0.0f, 0.0f, 0.0f);
	LightBuffer* m_lightBuffer = nullptr;
};
