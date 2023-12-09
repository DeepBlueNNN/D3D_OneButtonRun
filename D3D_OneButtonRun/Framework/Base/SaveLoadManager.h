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
	/// <summary>
	/// Save���� ImGUI ���� ����
	/// </summary>
	void Save();
	/// <summary>
	/// Scene���� ����: ���ϰ�� ����, Scene�̸�, Camera, Light, Scene�� ���ӿ��� ����
	/// </summary>
	/// <param name="savePath">Dialog�� ���� ���õ� ���</param>
	void SaveScene(wstring savePath);
	/// <summary>
	/// Scene�� Player�� ���͵� Type�� for���� ���� ���� ����
	/// </summary>
	/// <param name="gameActor">�ν��Ͻ� ���� ���ӿ���</param>
	/// <param name="actorType">���ӿ��� Ÿ��</param>
	/// <param name="count">�ν��Ͻ̵� ���� FBX ���� ����</param>
	/// <param name="type">���� XML���</param>
	void SaveActor(InstancingActor* gameActor, GameActor::GameActorTag actorType, int count, tinyxml2::XMLElement* type);

public:
	/// <summary>
	/// Load���� ImGUI ���� ����
	/// </summary>
	void Load();
	/// <summary>
	/// Scene���� �ε�: ���ϰ�� ����, Scene�̸�, Camera, Light, Scene�� ���ӿ��� ����, �� ���ӿ��� ����
	/// </summary>
	/// <param name="savePath">Dialog�� ���� ���õ� ���</param>
	void LoadScene(wstring savePath);
	/// <summary>
	/// �ε��� ���� ��� Set
	/// </summary>
	/// <param name="savePath">����� ���</param>
	void SetLoadFile(wstring savePath) { m_loadPath = savePath; }

public:
	/// <summary>
	/// Scene ȯ�� �ʱ�ȭ �� ��� ���ӿ��� ����
	/// </summary>
	void Clear();

public:
	void GUIRender();

public:
	GamePlayer*& GetPlayer() { return m_player; }
	/// <summary>
	/// �� Scene���� ����� �ν��Ͻ̵� GameActor�� ���۷��� Get 
	/// </summary>
	/// <returns>�ʱ�ȭ �Ǵ� �ε�� InstancedGameActor��</returns>
	vector<InstancingActor*>& GetInstancingActors() { return m_intancingActors; }
	/// <summary>
	/// ��� SkyDome ��������
	/// </summary>
	/// <returns>*& m_skyDome</returns>
	Sky*& GetSky() { return m_sky; }
	/// <summary>
	/// ����ī�޶� �޾ƿ���
	/// </summary>
	/// <returns>m_mainCamera</returns>
	Camera* GetMainCamera() { return m_mainCamera; }
	/// <summary>
	/// Environment���� ���� LightBuffer �޾ƿ���
	/// </summary>
	/// <returns>*&m_lightBuffer</returns>
	LightBuffer*& GetLightBuffer() { return m_lightBuffer; }
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
	/// <summary>
	/// ����� �÷��� ��� �޾ƿ���
	/// </summary>
	/// <returns>m_record</returns>
	vector<string> GetRecord() { return m_record; }
	wstring GetLoadPath() { return m_loadPath; }

private:
	// GameActors
	GamePlayer* m_player;
	vector<InstancingActor*> m_intancingActors;
	vector<string> m_classNames;
	vector<string> m_fbxNames;
	Sky* m_sky = nullptr;
	
	// SavePath
	wstring m_projectPath = L"";
	string m_savePath = "";

	// LoadPath
	wstring m_loadPath;

	// Record
	vector<string> m_record;

	// Environment
	Camera* m_mainCamera = nullptr;
	LightBuffer* m_lightBuffer = nullptr;
};
