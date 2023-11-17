#pragma once
class S02_Grid : public Scene
{
private:
	const int MAX_VALUE = 100;

public:
	S02_Grid();
	~S02_Grid();

public:
	void Update()      override;
	void Render()      override;
	void PreRender()   override;
	void PostRender()  override;
	void GUIRender()   override;

private:
	void CreateMesh();

private:
	MatrixBuffer*		m_worldBuffer = nullptr;
	UINT				m_width = MAX_VALUE;
	UINT				m_height = MAX_VALUE;
	Mesh<VertexColor>*	m_mesh = nullptr;
	Material*			m_material = nullptr;
};