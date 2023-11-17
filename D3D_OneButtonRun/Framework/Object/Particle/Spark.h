#pragma once
class Spark : public Particle
{
private:
	struct VertexParticle
	{
		Float3 position = { 0.0f, 0.0f, 0.0f };
		Float2 size		= { 0.0f, 0.0f };
		Float3 velocity = { 0.0f, 0.0f, 0.0f };
	};

public:
	Spark(wstring imageFile, bool isAdditive = false);
	~Spark();

public:
	void Update();
	void Render();
	void GUIRender();
	void Play(Vector3 position);

private:
	void Create();
	void UpdateParticle();

private:
	float m_maxRadius = 10.0f;
	float m_minRadius = 5.0f;

	float m_maxSize = 1.0f;
	float m_minSize = 0.1f;

private:
	vector<VertexParticle> m_vertices;

	FloatValueBuffer*	m_buffer = nullptr;
	ColorBuffer*		m_startColorBuffer = nullptr;
	ColorBuffer*		m_endColorBuffer = nullptr;

	VertexUV m_vertex;

};