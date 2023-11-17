#pragma once
class SpriteParticle : public Particle
{
private:
	class SpriteBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Vector2 maxFrame = Vector2(0.0f, 0.0f);
			Vector2 curFrame = Vector2(0.0f, 0.0f);
		};
	public:
		SpriteBuffer() : ConstBuffer(&m_data, sizeof(Data))
		{

		}
		Data& Get() { return m_data; }

	private:
		Data m_data;
	};

public:
	SpriteParticle(wstring imageFile,
		float width, float height,
		UINT frameCol, UINT frameRow,
		bool isAdditive = false);
	~SpriteParticle();

public:
	void Update();
	void Render();
	void GUIRender();
	void Play(Vector3 position);

private:
	void Create();

private:
	SpriteBuffer* m_buffer = nullptr;
	VertexUV	  m_vertex;

private:
	float m_time = 0.0f;
	float m_speed = 1.0f;

	UINT  m_curFrameCount = 0;
	UINT  m_maxFrameCount = 0;

	Vector2 m_size = Vector2(0.0f, 0.0f);

};