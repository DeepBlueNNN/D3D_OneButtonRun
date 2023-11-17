#pragma once
class Sphere : public GameObject
{
private:
	typedef VertexUVNormalTangent  VertexType;
	const  float MAX_HEIGHT = 20.0f;

public:
	Sphere(wstring strShader = L"", float radius = 1.0f, UINT sliceCount = 32, UINT statckCount = 16);
	~Sphere();

public:
	void Render();

private:
	void  MakeMesh();
	void  MakeTangent();

private:
	Mesh<VertexType>* m_mesh = nullptr;
	float	m_radius = 0.0f;
	UINT	m_sliceCount = 0;
	UINT	m_stackCount = 0;
};