#pragma once

class InstancingActor : public GameActor
{
public:
	InstancingActor(GameActorTag actorType, string fbxName);
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

	// Add&Erase Actor, Collider
	UINT Add();
	UINT AddCollider(UINT index, Collider::Type type);
	void Erase(UINT index);

private:
	vector<vector<Collider*>> m_colliders;
};
