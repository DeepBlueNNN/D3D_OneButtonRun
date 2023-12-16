#pragma once

class Scene_MapEditor : public Scene
{
private:
	struct StoreActor
	{
		InstancingActor* actor = nullptr;
		Transform* transform = nullptr;
		int index = 0;
	};

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
	/// ���õ� Actor�� Collider �߰�
	/// </summary>
	/// <param name="actor"></param>
	/// <param name="index"></param>
	/// <param name="type"></param>
	void AddCollider(InstancingActor* actor, UINT index);
	/// <summary>
	/// Scene_MapEditor runtime�� Actor����
	/// </summary>
	/// <param name="actor">GameActor*</param>
	void DeleteActor(InstancingActor* actor, UINT index);
	/// <summary>
	/// Collider* ����
	/// </summary>
	/// <param name="colliders"></param>
	/// <param name="index"></param>
	void DeleteCollider(InstancingActor* actor, vector<Collider*>& colliders, UINT index);

private:
	// Editor Variable
	//GameActor* m_selectedActor = nullptr;
	StoreActor m_selectedActor;
	UINT m_colliderType = 0;
	unordered_map<string, UINT> m_fbxList;
	Vector3 m_actorPos;
};