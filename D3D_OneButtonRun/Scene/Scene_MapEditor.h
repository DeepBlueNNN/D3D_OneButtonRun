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
	/// ���콺�� Collider Ŭ�� �� imgui Selected Actor �ǿ��� ���� ����
	/// </summary>
	void SelectActor();
	/// <summary>
	/// Scene_MapEditor runtime�� Actor�߰�
	/// </summary>
	/// <param name="type">GameActor::GameActorTag</param>
	void AddActor();
	/// <summary>
	/// Scene_MapEditor runtime�� Actor����
	/// </summary>
	/// <param name="actor">GameActor*</param>
	void DeleteActor(GameActor* actor);

private:
	// Editor Variable
	//GameActor* m_selectedActor = nullptr;
	Transform* m_selectedActor = nullptr;
	SphereCollider* m_playerStart;
};