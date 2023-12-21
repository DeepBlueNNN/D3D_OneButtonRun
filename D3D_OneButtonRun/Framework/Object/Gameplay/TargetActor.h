#pragma once
class TargetActor : public GameActor
{
public:
	TargetActor();
	~TargetActor();

public:
	virtual void Update()	  override;
	virtual void Render()	  override;
	virtual void PreRender()  override {}
	virtual void PostRender() override {}
	virtual void GUIRender()  override;

public:
	void SetPosition(Vector3 pos)
	{
		m_collider->Pos() = pos;
		m_model->Pos() = m_collider->Pos();
	}
	void SetRotation(Vector3 rot)
	{
		m_collider->Rot() = rot;
		m_model->Pos() = m_collider->Pos() + (m_collider->Down() * m_offset);
		m_model->Rot() = rot;
	}

	void ParticleOn(bool isOn);

private:
	Spark* m_spark = nullptr;
};