#include "framework.h"
#include "Model.h"
#define  INI_READER 0
  
Model::Model(string name)
{
	m_name = name;
	ReadMaterial();
	ReadMesh();

	m_worldBuffer = new MatrixBuffer();
}

Model::~Model()
{
	SAFE_DELETE(m_worldBuffer);

	//for (Material* matrial : m_materials)
	//	SAFE_DELETE(matrial);
	for (ModelMesh* mesh : m_meshes)
		SAFE_DELETE(mesh);

	m_materials.erase(m_materials.begin(), m_materials.end());
	m_meshes.erase(m_meshes.begin(), m_meshes.end());
}

void Model::Update()
{
	UpdateWorld();
}

void Model::Render()
{
	// HLSL파일이 같다면 Shader는 한개만 만들어지기 때문에 Update가 아닌 Render에서 WorldBuffer Set
	// Update과정에서 worldBuffer의 Shader가 영향을 받았을 때 원위치 시키는 목적
	m_worldBuffer->Set(m_world);
	m_worldBuffer->SetVS(0);  // W(0),V(1),P(1);

	for (ModelMesh* mesh : m_meshes)
		mesh->Render();
}

void Model::GUIRender()
{
	Transform::GUIRender();

	for (Material* material : m_materials)
		material->GUIRender();
}

void Model::SetShader(wstring file)
{
	for (Material* material : m_materials)
		material->SetShader(file);
}

void Model::ReadMaterial()
{
	string file = "Assets/Materials/" + m_name + "/" + m_name + ".mats";
	// ini방식
	string ini = "Assets/Materials/" + m_name + "/" + m_name + ".ini";

	if (INI_READER != 1)
	{
		BinaryReader reader(file);
		if (reader.IsFailed())
			assert(false);

		UINT size = reader.UInt();
		m_materials.reserve(size);

		for (UINT i = 0; i < size; i++)
		{
			Material* material = new Material();
			material->Load(reader.String());
			m_materials.push_back(material);
		}
	}
	else
	{
		// Section 읽기
		char buf[256];
		string temp;
		memset(buf, 0x00, sizeof(char));
	
		GetPrivateProfileStringA("Materials", "Count","",buf,256,ini.c_str());
		temp = buf;
		int  Count = stoi(temp);

		for (int i = 0; i < Count; i++)
		{
			memset(buf, 0x00, sizeof(char));
			temp = "Material_" + to_string(i + 1);
			GetPrivateProfileStringA(temp.c_str(), "FileName", "", buf, 256, ini.c_str());

			temp = buf;

			Material* material = new Material();
			material->Load(temp);
			m_materials.push_back(material);
		}
	}
}

void Model::ReadMesh()
{
	string file = "Assets/Meshes/" + m_name + "/" + m_name + ".mesh";

	BinaryReader reader(file);
	if (reader.IsFailed())
		assert(false);

	// Mesh 크기
	UINT size = reader.UInt();
	m_meshes.reserve(size);

	// Mesh 데이터
	for (UINT i=0; i<size; i++)
	{
		ModelMesh* mesh = new ModelMesh(reader.String());
		mesh->SetMaterial(m_materials[reader.UInt()]);

		UINT vertexCount = reader.UInt();

		int  ea = sizeof(ModelVertex) * vertexCount;
		char* data = new char[ea];

		reader.Byte((void**)&data, ea);

		UINT indexCount = reader.UInt();
		UINT* indices = new UINT[indexCount];
		reader.Byte((void**)&indices, sizeof(UINT) * indexCount);

		mesh->CreateMesh(data, vertexCount, indices, indexCount);

		m_meshes.push_back(mesh);

		SAFE_DELETE_ARRAY(data);
		SAFE_DELETE_ARRAY(indices);
	}

	// 애니메이션
	size = reader.UInt();
	m_nodes.resize(size);

	for (NodeData& node : m_nodes)
	{
		node.index		= reader.Int();
		node.name		= reader.String();
		node.parent		= reader.Int();
		node.transform	= reader.Matrix();
	}

	size = reader.UInt();
	m_bones.resize(size);

	for (BoneData& bone : m_bones)
	{
		bone.index		= reader.Int();
		bone.name		= reader.String();
		bone.offset		= reader.Matrix();

		m_boneMap[bone.name] = bone.index;
	}
}
