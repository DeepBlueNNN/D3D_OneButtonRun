#pragma once
class Quad : public GameObject
{
private:
	typedef VertexUV VertexType;

public:
	Quad(Vector2 size);
	Quad(wstring file);
	~Quad();

public:
	void Render();
	void SetRender();

private:
	void MakeMesh();

private:
	Vector2 m_size = Vector2(0.0f, 0.0f);
	Mesh<VertexType>* m_mesh = nullptr;

};