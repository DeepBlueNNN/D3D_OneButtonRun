#pragma once

class Scene_GamePlay : public Scene
{
public:
	Scene_GamePlay();
	~Scene_GamePlay();

public:
	void Update()      override;
	void Render()      override;
	void PreRender()   override;
	void PostRender()  override;
	void GUIRender()   override;

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
	UINT m_refreshCount = 0;
	float m_playTime = 0.0f;
	SubMenu* m_subMenu = nullptr;

};