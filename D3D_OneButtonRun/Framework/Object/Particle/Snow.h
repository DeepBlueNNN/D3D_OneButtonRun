#pragma once
class Snow : public Particle
{
private:
	struct VertexSnow
	{
		Float3 pos;
		Float2 size;
		Float2 random;
	};

public:
	Snow();
	~Snow();

public:
	void Update();
	void Render();
	void GUIRender();

private:
	void Create();

private:
	vector<VertexSnow>	m_vertices;
	WeatherBuffer*		m_buffer = nullptr;

};