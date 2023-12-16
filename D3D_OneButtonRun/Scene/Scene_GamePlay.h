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

private:
	GamePlayer* m_player = nullptr;
	vector<GameActor*> m_actors;

	vector<BoxCollider*> m_grounds;
	vector<Cube*> m_groundCubes;

private:
	Vector3 m_playerOriginPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_playerOriginVelocity = Vector3(0.0f, 0.0f, 0.0f);

	bool m_isPlayStart = true;	// Play Start �ÿ� 3�� ī��Ʈ flag
	UINT m_playStartCount = 3;

	bool m_isPlaying = false;	// 3�� ī��Ʈ �Ŀ� Game Play flag
	UINT m_refreshCount = 0;
	float m_playTime = 0.0f;

	SubMenu* m_subMenu = nullptr;
	HowToPlay* m_howToPlay = nullptr;

};