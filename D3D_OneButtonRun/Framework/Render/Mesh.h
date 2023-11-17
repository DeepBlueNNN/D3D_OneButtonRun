#pragma once

#include   "framework.h"

template<typename T>
class Mesh
{
public:
	Mesh() = default;  // 시스템에서 사용되는 기본생성자를 사용 : ShallowCopy
	~Mesh();

	void Draw(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void DrawInstanced(UINT instanceCount, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void CreateMesh();
	void UpdateVertex(void* data, UINT count);
	void UpdateVertex();
	void UpdateIndex();

	vector<T>&    GetVertices() { return m_vertices; }
	vector<UINT>& GetIndices()  { return m_indices; }
	VertexBuffer* GetVertexBuffer() { return m_vertexBuffer; }
	IndexBuffer*  GetIndexBuffer()  { return m_indexBuffer; }


private:
	VertexBuffer*	m_vertexBuffer = nullptr;
	IndexBuffer*	m_indexBuffer = nullptr;
	vector<T>		m_vertices;
	vector<UINT>	m_indices;
};

template<typename T>
inline Mesh<T>::~Mesh()
{
	SAFE_DELETE(m_vertexBuffer);
	SAFE_DELETE(m_indexBuffer);
}

template<typename T>
inline void Mesh<T>::Draw(D3D11_PRIMITIVE_TOPOLOGY type)
{
	m_vertexBuffer->Set(type);
	if (m_indexBuffer)
	{
		m_indexBuffer->Set();
		DC->DrawIndexed((UINT)m_indices.size(), 0, 0);
	}
	else
	{
		DC->Draw((UINT)m_vertices.size(), 0);
	}
}

template<typename T>
inline void Mesh<T>::DrawInstanced(UINT instanceCount, D3D11_PRIMITIVE_TOPOLOGY type)
{
	m_vertexBuffer->Set(type);
	if (m_indexBuffer)
	{
		m_indexBuffer->Set();
		DC->DrawIndexedInstanced((UINT)m_indices.size(), instanceCount, 0, 0, 0);
	}
	else
	{
		DC->DrawInstanced((UINT)m_vertices.size(), instanceCount, 0, 0);
	}
}

template<typename T>
inline void Mesh<T>::CreateMesh()
{
	if (m_vertices.size() > 0)
	{
		m_vertexBuffer = new VertexBuffer(m_vertices.data(),
			sizeof(T), (UINT)m_vertices.size());
	}

	if (m_indices.size() > 0)
	{
		m_indexBuffer = new IndexBuffer(m_indices.data(),
			(UINT)m_indices.size());
	}
}

template<typename T>
inline void Mesh<T>::UpdateVertex(void* data, UINT count)
{
	m_vertexBuffer->Update(data, count);
}

template<typename T>
inline void Mesh<T>::UpdateVertex()
{
	m_vertexBuffer->Update(m_vertices.data(), m_vertices.size());
	m_vertexBuffer->Set();
}

template<typename T>
inline void Mesh<T>::UpdateIndex()
{
	m_indexBuffer->Update(m_indices.data(), m_indices.size());
	m_indexBuffer->Set();
}
