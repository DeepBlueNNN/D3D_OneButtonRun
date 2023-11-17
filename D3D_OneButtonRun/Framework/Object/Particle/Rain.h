#pragma once
class Rain : public Particle
{
public:
	Rain();
	~Rain();

public:
	void Update();
	void Render();
	void GUIRender();

private:
	void Create();

private:
	vector<VertexUV> m_vertices;
	WeatherBuffer*   m_buffer = nullptr;

};