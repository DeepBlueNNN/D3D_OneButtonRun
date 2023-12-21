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
	/// <summary>
	/// m_model�� ModelAnimator�� ��ȯ�Ͽ� ��ȯ�ϴ� Getter. 
	/// </summary>
	/// <returns></returns>
	ModelAnimator* GetModelAnim() { return dynamic_cast<ModelAnimator*>(m_model); }
	/// <summary>
	/// ���� player�� �浹�� �ݶ��̴��� ��Ƴ��� ����(set)�� ��ȯ�ϴ� Getter. 
	/// </summary>
	/// <returns></returns>
	set<Collider*> GetCollidedObjects() { return m_collidedObjects; }
	
	/// <summary>
	/// �߷� ���뿩�� flag�� ��ȯ�ϴ� Getter. 
	/// </summary>
	/// <returns></returns>
	bool GetIsGravityActive() { return m_isGravityActive; }

public:
	/// <summary>
	/// ���� ����Ǿ� �ִ� velocity�� ��ȯ�ϴ� Getter. 
	/// </summary>
	/// <returns></returns>
	Vector3& Velocity() { return m_velocity; }

public:
	/// <summary>
	/// �浹 ���θ� ��Ÿ���� flag�� ���� �����ϴ� Setter. 
	/// </summary>
	/// <param name="isCollision"> flag�� �������� ��</param>
	void SetIsCollision(bool isCollision) { m_isCollision = isCollision; }
	/// <summary>
	/// �߷� ���뿩�θ� ��Ÿ���� flag�� ���� �����ϴ� Setter. 
	/// </summary>
	/// <param name="isGravityActive"> flag�� �������� ��</param>
	void SetIsGravityActive(bool isGravityActive) { m_isGravityActive = isGravityActive; }

	/// <summary>
	/// ���ο� ��ġ�������� �ݶ��̴��� ���� ��ġ������ �����ִ� Setter. 
	/// </summary>
	/// <param name="pos"> ���Ӱ� ������ ��ġ��</param>
	void	SetPosition(Vector3 pos)
	{
		m_collider->Pos() = pos;
		m_model->Pos() = m_collider->Pos();
	}
	/// <summary>
	/// ���ο� ȸ�������� �ݶ��̴��� ���� ȸ����Ű�� Setter. 
	/// ���� (offset * �ݶ��̴��� Down Vector) �� ��ŭ ���� ��ġ�� �����ش�.
	/// </summary>
	/// <param name="rot"> ���Ӱ� ȸ���� ��</param>
	void	SetRotation(Vector3 rot)
	{
		m_collider->Rot() = rot;
		m_model->Pos() = m_collider->Pos() + (m_collider->Down() * m_offset);
		m_model->Rot() = rot;
	}
	/// <summary>
	/// player�� ��ġ�� ȸ������ ZeroVector�� �ʱ�ȭ ��Ű�� �Լ�. 
	/// </summary>
	void InitTransform()
	{
		SetPosition(Vector3(0, 0, 0));
		SetRotation(Vector3(0, 0, 0));
		m_rotValue = Vector3(0, 0, 0);
	}

	void SetFrictionAnimToIdle();

public:
	/// <summary>
	/// player�� object�� �浹 �ÿ� ȣ��. 
	/// �ݹ߰� ������ ����Ͽ� ����. 
	/// </summary>
	/// <param name="closestPoint"> �浹 ����</param>
	void Friction(Vector3 closestPoint);

public:
	/// <summary>
	/// player�� �浹�� object�� ����(set)�� Insert�ϴ� �Լ�. 
	/// </summary>
	/// <param name="object"> �浹�� Object</param>
	void RegisterObject(Collider* object);
	/// <summary>
	/// player�� �浹�ߴ� object�� ���տ��� �����ϴ� �Լ�. 
	/// </summary>
	/// <param name="object"> ������ Object</param>
	void RemoveObject(Collider* object);

private:
	bool m_isCollision = false;
	bool m_isGravityActive = false;

private:
	Vector3 m_velocity = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 m_rotValue = Vector3(0.0f, 0.0f, 0.0f);

private:
	const Vector3 GRAVITY = Vector3(0.0f, -9.8f, 0.0f) * 1.5f;

private:
	set<Collider*> m_collidedObjects;

};