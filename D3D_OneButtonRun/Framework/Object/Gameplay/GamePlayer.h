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
	Vector3& Velocity() { return m_velocity; }
	Vector3& PrevPos()  { return m_prevPos; }
	Vector3& CurPos()   { return m_curPos; }

public:
	void SetIsCollision(bool isCollision) { m_isCollision = isCollision; }

	void	SetPosition(Vector3 pos)
	{
		m_collider->Pos() = pos;
		m_model->Pos() = m_collider->Pos();
	}
	void	SetRotation(Vector3 rot)
	{
		m_collider->Rot() = rot;
		m_model->Pos() = m_collider->Pos() + (m_collider->Down() * m_offset);
		m_model->Rot() = rot;
	}

public:
	void Friction(Vector3 closestPoint);

private:
	bool m_isCollision = false;

private:
	Vector3 m_prevPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_curPos  = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_velocity = Vector3(0.0f, 10.0f, 0.0f);
	Vector3 m_rotValue = Vector3(0.0f, 0.0f, 0.0f);

private:
	const Vector3 GRAVITY = Vector3(0.0f, -9.8f, 0.0f);

};