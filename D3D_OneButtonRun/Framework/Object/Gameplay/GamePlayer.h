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
	/// m_model을 ModelAnimator로 변환하여 반환하는 Getter. 
	/// </summary>
	/// <returns></returns>
	ModelAnimator* GetModelAnim() { return dynamic_cast<ModelAnimator*>(m_model); }
	/// <summary>
	/// 현재 player와 충돌한 콜라이더를 모아놓은 집합(set)을 반환하는 Getter. 
	/// </summary>
	/// <returns></returns>
	set<Collider*> GetCollidedObjects() { return m_collidedObjects; }
	
	/// <summary>
	/// 중력 적용여부 flag를 반환하는 Getter. 
	/// </summary>
	/// <returns></returns>
	bool GetIsGravityActive() { return m_isGravityActive; }

public:
	/// <summary>
	/// 현재 적용되어 있는 velocity를 반환하는 Getter. 
	/// </summary>
	/// <returns></returns>
	Vector3& Velocity() { return m_velocity; }

public:
	/// <summary>
	/// 충돌 여부를 나타내는 flag의 값을 설정하는 Setter. 
	/// </summary>
	/// <param name="isCollision"> flag에 설정해줄 값</param>
	void SetIsCollision(bool isCollision) { m_isCollision = isCollision; }
	/// <summary>
	/// 중력 적용여부를 나타내는 flag의 값을 설정하는 Setter. 
	/// </summary>
	/// <param name="isGravityActive"> flag에 설정해줄 값</param>
	void SetIsGravityActive(bool isGravityActive) { m_isGravityActive = isGravityActive; }

	/// <summary>
	/// 새로운 위치지점으로 콜라이더와 모델의 위치지점을 맞춰주는 Setter. 
	/// </summary>
	/// <param name="pos"> 새롭게 설정할 위치점</param>
	void	SetPosition(Vector3 pos)
	{
		m_collider->Pos() = pos;
		m_model->Pos() = m_collider->Pos();
	}
	/// <summary>
	/// 새로운 회전값으로 콜라이더와 모델을 회전시키는 Setter. 
	/// 모델은 (offset * 콜라이더의 Down Vector) 값 만큼 현재 위치에 더해준다.
	/// </summary>
	/// <param name="rot"> 새롭게 회전할 값</param>
	void	SetRotation(Vector3 rot)
	{
		m_collider->Rot() = rot;
		m_model->Pos() = m_collider->Pos() + (m_collider->Down() * m_offset);
		m_model->Rot() = rot;
	}
	/// <summary>
	/// player의 위치와 회전값을 ZeroVector로 초기화 시키는 함수. 
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
	/// player가 object와 충돌 시에 호출. 
	/// 반발과 마찰을 계산하여 적용. 
	/// </summary>
	/// <param name="closestPoint"> 충돌 지점</param>
	void Friction(Vector3 closestPoint);

public:
	/// <summary>
	/// player와 충돌한 object를 집합(set)에 Insert하는 함수. 
	/// </summary>
	/// <param name="object"> 충돌한 Object</param>
	void RegisterObject(Collider* object);
	/// <summary>
	/// player와 충돌했던 object를 집합에서 삭제하는 함수. 
	/// </summary>
	/// <param name="object"> 삭제할 Object</param>
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