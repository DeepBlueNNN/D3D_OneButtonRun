#pragma once
class GameActor
{
public:
	enum GameActorTag
	{
		PLAYER = 0,
		TREE,
		ROCK,
		GROUND,
		MAX
	};

public:
	GameActor(GameActorTag type);
	~GameActor() = default;

public:
	// Getter
	Collider*	GetCollider()	{ return m_collider; }
	Model*		GetModel()		{ return m_model; }

	Vector3		GetModelPosition()	{ return m_model->Pos(); }
	Vector3		GetModelRotation()	{ return m_model->Rot(); }
	Vector3		GetModelScale()		{ return m_model->Scale(); }

	Vector3		GetColliderPosition() { return m_collider->Pos(); }
	Vector3		GetColliderRotation() { return m_collider->Rot(); }
	Vector3		GetColliderScale()	  { return m_collider->Scale(); }

	Vector3		GetGlobalPosition() { return m_collider->GlobalPos(); }

	GameActorTag  GetGameActorType() { return m_type; }

	string	GetName() { return m_name; }

	bool	GetActive() { return m_isActive; }

public:
	// Setter
	void	SetModelScale(Vector3 scale)	{ m_model->Scale() = scale; }
	void	SetColliderScale(Vector3 scale) { m_collider->Scale() = scale; }
	void	SetName(string name) { m_name = name; }
	void	SetGameActorType(GameActorTag type) { m_type = type; }

	// Set GameActor Active
	void	SetActive(bool isActive) { m_isActive = isActive; }

public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void PreRender() = 0;
	virtual void PostRender() = 0;
	virtual void GUIRender() = 0;

protected:
	Model*		m_model = nullptr;
	Collider*	m_collider = nullptr;

	string	m_name = "";
	GameActorTag m_type = MAX;
	bool	m_isActive = true;
	float	m_offset = 0.0f;

};