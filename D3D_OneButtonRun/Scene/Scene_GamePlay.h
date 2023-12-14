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