#pragma once

class Sky : public Transform
{
public:
	Sky(wstring key);
	~Sky();

public:
	void Render();
	void GUIRender();

public:
	wstring GetCurrentTextureFile() { return m_cubeTexture->GetFile(); }

public: // Setter
	void SetTexture(wstring textureFile);

private:
	Sphere*				m_sphere = nullptr;
	Texture*			m_cubeTexture = nullptr;
	vector<wstring>		m_skyTextureFiles;
	vector<string>		m_comboList;
	RasterizerState*	m_rasterizerState[2];
	DepthStencilState*	m_depthStencilState[2];
};
