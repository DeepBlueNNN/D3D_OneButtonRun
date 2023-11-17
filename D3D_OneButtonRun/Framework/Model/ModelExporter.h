#pragma once
class  Material;
class  ModelExporter
{
public:
	struct  Weight
	{
		int   vID;
		float weight;
	};
public:
	ModelExporter(string name, string file);

public:
	void  ExportMaterial();
	void  ExportMesh();
	void  ExportClip(string clipName);

private:  // Materail
	void  ReadMaterial();
	void  WriteMaterial();
	string CreateTexture(string file);   // Texture파일확장자(pda, tga) png파일로 변환

private:  // Mesh
	void  ReadMesh(aiNode* node);
	void  ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeight);
	void  ReadNode(aiNode* node, int index, int parent);
	void  WriteMesh();

private:
	Clip* ReadClip(aiAnimation* animation);
	void  ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes);
	void  WriterClip(Clip* clip, string clipName, UINT index);

private:
	Assimp::Importer*	m_importer = nullptr;
	const  aiScene*		m_scene   = nullptr;

private:
	vector<Material*>	m_materials;
	vector<MeshData*>	m_meshes;
	string				m_name = "";

private:
	vector<NodeData*>	m_nodes;
	vector<BoneData*>	m_bones;
	map<string, UINT>	m_boneMap;
	UINT				m_boneCount = 0;

};
