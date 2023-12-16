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
	/// 마우스로 Collider 클릭 시 imgui Selected Actor 탭에서 편집 가능
	/// </summary>
	void SelectActor();
	/// <summary>
	/// Scene_MapEditor runtime에 Actor추가
	/// </summary>
	/// <param name="type">GameActor::GameActorTag</param>
	void AddActor();
	/// <summary>
	/// 선택된 Actor에 Collider 추가
	/// </summary>
	/// <param name="actor"></param>
	/// <param name="index"></param>
	/// <param name="type"></param>
	void AddCollider(InstancingActor* actor, UINT index);
	/// <summary>
	/// Scene_MapEditor runtime에 Actor삭제
	/// </summary>
	/// <param name="actor">GameActor*</param>
	void DeleteActor(InstancingActor* actor, UINT index);
	/// <summary>
	/// Collider* 삭제
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