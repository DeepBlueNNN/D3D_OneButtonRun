#pragma once
class  Cube : public GameObject
{
private:
	typedef VertexUV  VertexType;

public:
	Cube(wstring strShader = L"", Vector3 size = Vector3(1.0f, 1.0f, 1.0f));
	~Cube();

public:
	void Render() override;

private:
	void CreateVertex(Vector3 size);

private:
	Mesh<VertexType>* m_mesh = nullptr;
};