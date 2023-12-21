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
	Spark(wstring imageFile, bool isAdditive = false, bool isLoop = false);
	~Spark();

public:
	void Update(Vector3 position);
	void Render();
	void GUIRender();
	void Play(Vector3 position);

public:	//Setter
	void SetLoop(bool isLoop) { m_isLoop = isLoop; }
	void SetSizeRange(float min, float max);
	void SetRadiusRange(float min, float max);
	void SetColor(Float4 start, Float4 end);

private:
	void Create();
	void UpdateParticle();

private:
	bool m_isLoop = false;

	float m_maxRadius = 5.0f;
	float m_minRadius = 4.0f;

	float m_maxSize = 1.0f;
	float m_minSize = 0.1f;

private:
	vector<VertexParticle> m_vertices;

	FloatValueBuffer*	m_buffer = nullptr;
	ColorBuffer*		m_startColorBuffer = nullptr;
	ColorBuffer*		m_endColorBuffer = nullptr;

	VertexUV m_vertex;

};