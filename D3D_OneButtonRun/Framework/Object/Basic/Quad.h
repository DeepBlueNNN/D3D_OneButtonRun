#pragma once
class Quad : public GameObject
{
protected:
	typedef VertexUV VertexType;

public:
	Quad(Vector2 size);
	Quad(wstring file);
	Quad(wstring file, Vector2 size);
	~Quad();

public:
	void Render();
	void SetRender();

private:
	void MakeMesh();

protected:
	Vector2 m_size = Vector2(0.0f, 0.0f);
	Mesh<VertexType>* m_mesh = nullptr;


};