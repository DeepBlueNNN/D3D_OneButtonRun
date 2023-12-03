#pragma once

class InstancingActor : public GameActor
{
public:
	InstancingActor(GameActorTag actorType, string fbxName, Collider::Type colliderType);
	~InstancingActor();

public:
	virtual void Update();
	virtual void Render();
	virtual void PreRender() {}
	virtual void PostRender() {}
	virtual void GUIRender();

public:
	ModelInstancing* GetModels() { return dynamic_cast<ModelInstancing*>(m_model); }
	vector<Collider*> GetColliders() { return m_colliders; }
	Collider::Type GetColliderType() { return m_colliderType; }

	int Add();
	void Erase(int index);

private:
	vector<Collider*> m_colliders;
	Collider::Type m_colliderType;
};
