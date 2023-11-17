#pragma once
class  Model : public Transform
{
public:
	Model(string name);
	~Model();

public:
	void  Update();
	void  Render();
	void  GUIRender();
	void  SetShader(wstring file);

private:
	void  ReadMaterial();
	void  ReadMesh();

protected:
	string m_name = "";

	vector<Material*>  m_materials;
	vector<ModelMesh*> m_meshes;

	vector<NodeData>   m_nodes;
	vector<BoneData>   m_bones;
	map<string, UINT>  m_boneMap;

	MatrixBuffer*      m_worldBuffer = nullptr;
};
