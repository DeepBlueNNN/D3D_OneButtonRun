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
	/// Save관련 ImGUI 구성 설정
	/// </summary>
	void Save();
	/// <summary>
	/// Scene정보 저장: 파일경로 설정, Scene이름, Camera, Light, Scene내 게임엑터 갯수
	/// </summary>
	/// <param name="savePath">Dialog를 통해 선택된 경로</param>
	void SaveScene(wstring savePath);
	/// <summary>
	/// Scene내 Player외 엑터들 Type별 for문을 통해 정보 저장
	/// </summary>
	/// <param name="gameActor">인스턴싱 단위 게임엑터</param>
	/// <param name="actorType">게임엑터 타입</param>
	/// <param name="count">인스턴싱된 동일 FBX 엑터 갯수</param>
	/// <param name="type">상위 XML노드</param>
	void SaveActor(InstancingActor* gameActor, GameActor::GameActorTag actorType, int count, tinyxml2::XMLElement* type);

public:
	/// <summary>
	/// Load관련 ImGUI 구성 설정
	/// </summary>
	void Load();
	/// <summary>
	/// Scene정보 로드: 파일경로 설정, Scene이름, Camera, Light, Scene내 게임엑터 갯수, 각 게임엑터 정보
	/// </summary>
	/// <param name="savePath">Dialog를 통해 선택된 경로</param>
	void LoadScene(wstring savePath);
	/// <summary>
	/// 로드할 파일 경로 Set
	/// </summary>
	/// <param name="savePath">저장된 경로</param>
	void SetLoadFile(wstring savePath) { m_loadPath = savePath; }

public:
	/// <summary>
	/// Scene 환경 초기화 및 모든 게임엑터 삭제
	/// </summary>
	void Clear();

public:
	void GUIRender();

public:
	GamePlayer*& GetPlayer() { return m_player; }
	/// <summary>
	/// 각 Scene에서 사용할 인스턴싱된 GameActor들 레퍼런스 Get 
	/// </summary>
	/// <returns>초기화 또는 로드된 InstancedGameActor들</returns>
	vector<InstancingActor*>& GetInstancingActors() { return m_intancingActors; }
	/// <summary>
	/// 배경 SkyDome 가져오기
	/// </summary>
	/// <returns>*& m_skyDome</returns>
	Sky*& GetSky() { return m_sky; }
	/// <summary>
	/// 메인카메라 받아오기
	/// </summary>
	/// <returns>m_mainCamera</returns>
	Camera* GetMainCamera() { return m_mainCamera; }
	/// <summary>
	/// Environment에서 사용될 LightBuffer 받아오기
	/// </summary>
	/// <returns>*&m_lightBuffer</returns>
	LightBuffer*& GetLightBuffer() { return m_lightBuffer; }
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
	/// <summary>
	/// 저장된 플레이 기록 받아오기
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
