#pragma once
class  ModelMesh
{
public:
	ModelMesh(string name);
	~ModelMesh();

public:
	void  Render();
	void  RenderInstanced(UINT instanceCount);
	void  CreateMesh(void* vertexData, UINT vertexCount, void* indexData, UINT indexCount);
	void  SetMaterial(Material* material) { m_material = material; }


private:
	string m_name = "";

	Material*			m_material = nullptr;
	Mesh<ModelVertex>*	m_mesh;
};