#pragma once

class Scene_GamePlay : public Scene
{
public:
	Scene_GamePlay();
	~Scene_GamePlay();

public:
	void Update()      override;
	void Render()      override;
	void ChangeScene() override;
	void PreRender()   override;
	void PostRender()  override;
	void GUIRender()   override;

public:
	/// <summary>
	/// 게임 플레이 데이터 초기화 함수
	/// </summary>
	void Initialize();

private:
	/// <summary>
	/// RefreshCount를 출력해주는 함수. 
	/// </summary>
	void PrintRefreshCount();
	/// <summary>
	/// PlayTime을 출력해주는 함수. 
	/// </summary>
	void PrintPlayTime();

	/// <summary>
	/// float 값의 소수점 2번째자리까지 잘라주는 함수. 
	/// </summary>
	/// <param name="value"> 소수점을 잘라줄 float 값</param>
	/// <returns></returns>
	string ConvertToString(float value);

	/// <summary>
	/// 게임 클리어 시에 기록을 저장하는 함수
	/// </summary>
	void RecordSave();

private:
	/*GamePlayer* m_player = nullptr;
	vector<InstancingActor*> m_actors;
	TargetActor* m_targetActor = nullptr;*/

private:
	Vector3 m_playerOriginPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_playerOriginVelocity = Vector3(0.0f, 0.0f, 0.0f);

	bool m_isPlayStart = true;	// Play Start 시에 3초 카운트 flag
	UINT m_playStartCount = 3;

	bool m_isPlaying = false;	// 3초 카운트 후에 Game Play flag
	UINT m_refreshCount = 0;
	float m_playTime = 0.0f;

	bool m_isStageClear = false;

private:
	SubMenu* m_subMenu = nullptr;
	HowToPlay* m_howToPlay = nullptr;
	ClearUI* m_clearUI = nullptr;
};