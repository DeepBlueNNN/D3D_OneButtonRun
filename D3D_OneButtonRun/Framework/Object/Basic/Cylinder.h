#pragma once
class Cylinder : public GameObject
{
private:
	typedef VertexUVNormalTangent  VertexType;
	const float MAX_HEIGHT = 20.0f;

public:
	Cylinder(wstring strShader = L"", float radius = 1.0f, float height = 1.0f, UINT sliceCount = 32);
	~Cylinder();

public:
	void Render() override;

private:
	void MakeMesh();
	void MakeNormal();
	void MakeTangent();

private:
	Mesh<VertexType>* m_mesh = nullptr;
	float	m_radius = 0.0f;
	float	m_height = 0.0f;
	UINT	m_sliceCount = 0;
};
