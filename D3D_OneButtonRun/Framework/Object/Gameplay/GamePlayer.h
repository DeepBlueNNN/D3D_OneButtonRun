#pragma once
class GamePlayer : public GameActor
{
public:
	GamePlayer();
	~GamePlayer();

public:
	virtual void Update()	  override;
	virtual void Render()	  override;
	virtual void PreRender()  override {}
	virtual void PostRender() override {}
	virtual void GUIRender()  override;

public:
	ModelAnimator* GetModelAnim() { return dynamic_cast<ModelAnimator*>(m_model); }

public:
	Vector3& Velocity() { return velocity; }
	Vector3& PrevPos()  { return prevPos; }
	Vector3& CurPos()   { return curPos; }

public:
	void SetIsCollision(bool isCollision) { m_isCollision = isCollision; }

public:
	void Friction(Vector3 closestPoint);

private:
	bool m_isCollision = false;

private:
	Vector3 prevPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 curPos  = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 velocity = Vector3(0.0f, 10.0f, 0.0f);

private:
	const Vector3 GRAVITY = Vector3(0.0f, -9.8f, 0.0f);

};