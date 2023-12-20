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
	/// ���� �÷��� ������ �ʱ�ȭ �Լ�
	/// </summary>
	void Initialize();

private:
	/// <summary>
	/// RefreshCount�� ������ִ� �Լ�. 
	/// </summary>
	void PrintRefreshCount();
	/// <summary>
	/// PlayTime�� ������ִ� �Լ�. 
	/// </summary>
	void PrintPlayTime();

	/// <summary>
	/// float ���� �Ҽ��� 2��°�ڸ����� �߶��ִ� �Լ�. 
	/// </summary>
	/// <param name="value"> �Ҽ����� �߶��� float ��</param>
	/// <returns></returns>
	string ConvertToString(float value);

	/// <summary>
	/// ���� Ŭ���� �ÿ� ����� �����ϴ� �Լ�
	/// </summary>
	void RecordSave();

private:
	/*GamePlayer* m_player = nullptr;
	vector<InstancingActor*> m_actors;
	TargetActor* m_targetActor = nullptr;*/

private:
	Vector3 m_playerOriginPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_playerOriginVelocity = Vector3(0.0f, 0.0f, 0.0f);

	bool m_isPlayStart = true;	// Play Start �ÿ� 3�� ī��Ʈ flag
	UINT m_playStartCount = 3;

	bool m_isPlaying = false;	// 3�� ī��Ʈ �Ŀ� Game Play flag
	UINT m_refreshCount = 0;
	float m_playTime = 0.0f;

	bool m_isStageClear = false;

private:
	SubMenu* m_subMenu = nullptr;
	HowToPlay* m_howToPlay = nullptr;
	ClearUI* m_clearUI = nullptr;
};