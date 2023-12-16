#pragma once

class InstancingActor : public GameActor
{
public:
	InstancingActor(GameActorTag actorType, string folder, string fbxName);
	~InstancingActor();

public:
	virtual void Update();
	virtual void Render();
	virtual void PreRender() {}
	virtual void PostRender() {}
	virtual void GUIRender();

public:
	// Getter
	ModelInstancing* GetModels() { return dynamic_cast<ModelInstancing*>(m_model); }
	vector<vector<Collider*>>& GetColliders() { return m_colliders; }
	string GetFolderName() { return m_folder; }

	// Add&Erase Actor, Collider
	UINT Add();
	void Erase(UINT index);
	UINT AddCollider(UINT index, Collider::Type type);
	void EraseCollider(vector<Collider*>& colliders, Collider* collider);
	void EraseCollider(vector<Collider*>& colliders, UINT index);
	void EraseCollider(UINT index, Collider* collider);
	void EraseCollider(UINT firstIndex, UINT secondIndex);

private:
	vector<vector<Collider*>> m_colliders;
	string m_folder = "";

};
