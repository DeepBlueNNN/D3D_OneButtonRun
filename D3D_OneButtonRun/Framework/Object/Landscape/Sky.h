#pragma once

class Sky : public Transform
{
public:
	Sky(wstring textureFile);
	~Sky();

public:
	void Render();

private:
	Sphere*				m_sphere = nullptr;
	Texture*			m_cubeMap = nullptr;
	RasterizerState*	m_rasterizerState[2];
	DepthStencilState*	m_depthStencilState[2];
};
