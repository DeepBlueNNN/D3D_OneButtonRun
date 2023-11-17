#pragma once

class Particle
{
protected:
	const UINT MAX_COUNT = 1000;

public:
	Particle();
	~Particle();

public:
	virtual void Update() {};
	virtual void Render();
	virtual void GUIRender() {};

	virtual void Play(Vector3 position);
	void Stop();

protected:
	bool	m_isActive = false;
	UINT	m_particleCount = 0;
	Vector3 m_position;

protected:
	Material*		 m_material = nullptr;
	VertexBuffer*	 m_vertexBuffer = nullptr;
	GeometryShader*	 m_geometryShader = nullptr;
	vector<VertexUV> m_vertices;

protected:
	BlendState*			 m_blendState[2];
	DepthStencilState*	 m_depthState[2];

};