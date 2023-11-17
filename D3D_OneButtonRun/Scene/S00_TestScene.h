#pragma once
class S00_TestScene : public Scene
{
public:
	S00_TestScene();
	~S00_TestScene();

public:
	void Update()      override;
	void Render()      override;
	void PreRender()   override;
	void PostRender()  override;
	void GUIRender()   override;

private:
	Cube*		m_cube = nullptr;
	Sphere*		m_sphere = nullptr;
	Cylinder*	m_cylinder = nullptr;

	vector<Collider*>	m_colliders;
	ModelAnimator*		m_modelAnimator = nullptr;
	ModelInstancing*	m_modelInstanced = nullptr;

	ProgressBar*	m_hpBar = nullptr;
	Particle*		m_particle = nullptr;
};