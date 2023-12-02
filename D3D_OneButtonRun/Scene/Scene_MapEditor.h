#pragma once

class Scene_MapEditor : public Scene
{
public:
	Scene_MapEditor();
	~Scene_MapEditor();

public:
	void Update()      override;
	void Render()      override;
	void PreRender()   override;
	void PostRender()  override;
	void GUIRender()   override;

private:
	/// <summary>
	/// 마우스로 Collider 클릭 시 imgui Selected Actor 탭에서 편집 가능
	/// </summary>
	void SelectActor();
	/// <summary>
	/// Scene_MapEditor runtime에 Actor추가
	/// </summary>
	/// <param name="type">GameActor::GameActorTag</param>
	void AddActor();
	/// <summary>
	/// Scene_MapEditor runtime에 Actor삭제
	/// </summary>
	/// <param name="actor">GameActor*</param>
	void DeleteActor(GameActor* actor);

private:
	// Editor Variable
	//GameActor* m_selectedActor = nullptr;
	Transform* m_selectedActor = nullptr;
	SphereCollider* m_playerStart;
};