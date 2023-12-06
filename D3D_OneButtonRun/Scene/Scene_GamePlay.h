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
	GamePlayer* m_player = nullptr;
	vector<GameActor*> m_actors;

	vector<BoxCollider*> m_grounds;
	vector<Cube*> m_groundCubes;

private:
	Vector3 m_playerOriginPos = Vector3(0.0f, 0.0f, 0.0f);

};