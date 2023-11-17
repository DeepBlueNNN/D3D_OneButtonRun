#include "framework.h"
#include "ModelExporter.h"

// ASSIMP 라이브러리의 데이터 구조
/*
	Importer
	ㄴScene
	  ㄴMaterial (Real Data)
		ㄴMesh (Real Data)
		  ㄴSceneRoot
			- Mesh 구조 (Unreal Skeletal Mesh의 BoneTree 같은 구조 : Root > Pelvis > Spine)
			- 각 본마다 Matrix, Material ID(인덱스), Mesh ID(vertex, index)정보가 담겨있음
			- Root본은 가상본데이터 허상본데이터 2개를 가지고 있다 (메쉬의 중심 용 허상, 실제 본으로서의 기능 데이터)
*/

ModelExporter::ModelExporter(string name, string file)
{
	m_name = name;

	m_importer = new Assimp::Importer();

	m_importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

	m_scene = m_importer->ReadFile(file,
		aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
	assert(m_scene != nullptr);


}

void ModelExporter::ExportMaterial()
{
	// 1. Material 읽기 > C:User/QWE~ FBX의 Material / Texture 정보 읽어옴
	// 2. Embeded된 Texture정보가 없다면 Color/White로 저장
	// 3. CreateTexture를 통해 각 Texture를 NM, SM, NM 별로 png파일에 저장
	ReadMaterial();

	// 4. Model/ Materials/모델명 에 XML파일 생성
	WriteMaterial();
}

void ModelExporter::WriteMaterial()
{
	string savePath = "Assets/Materials/" + m_name + "/";
	string file = m_name + ".mats";
	string ini = savePath + m_name  + ".ini";

	// 디렉토리 생성
	StringPath::CreateFolders(savePath);

	WritePrivateProfileStringA("Materials", "Count",
		to_string(m_materials.size()).c_str(), ini.c_str());

	BinaryWriter w(savePath + file);

	w.UInt(m_materials.size());

	for (UINT i=0; i<m_materials.size(); i++)
	{
		Material* material = m_materials.at(i);
		string path = savePath + material->GetName() + ".xml";
		material->Save(path); 
		string  temp = "Material_" + to_string(i + 1);

		WritePrivateProfileStringA(temp.c_str(), "FileName",
			path.c_str(), ini.c_str());
		w.String(path);
		SAFE_DELETE(m_materials[i]);
	}

	m_materials.erase(m_materials.begin(), m_materials.end());
}

void ModelExporter::ReadMaterial()
{
	for (UINT i = 0; i < m_scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = m_scene->mMaterials[i];
		Material*   material = new Material();
		m_materials.push_back(material);

		// matrial 명칭 저장
		material->GetName() = srcMaterial->GetName().C_Str();
		// Material Class에 있는 buffer의 데이타를 가져옴
		MaterialBuffer::Data& data = material->GetData();
		// color값 저장
		aiColor3D  color;
		{
			srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			data.diffuse = Float4(color.r, color.g, color.b, 1.0f);

			srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
			data.specular = Float4(color.r, color.g, color.b, 1.0f);

			srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
			data.ambient = Float4(color.r, color.g, color.b, 1.0f);

			srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
			data.emissive = Float4(color.r, color.g, color.b, 1.0f);
		}

		aiString file;
		wstring  imageFile;
		{
			srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
			imageFile = StringPath::ToWString(CreateTexture(file.C_Str()));

			material->SetDiffuseMap(imageFile);

			file.Clear();

			srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
			imageFile = StringPath::ToWString(CreateTexture(file.C_Str()));
			material->SetSpecularMap(imageFile);
			file.Clear();

			srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
			imageFile = StringPath::ToWString(CreateTexture(file.C_Str()));
			material->SetNormalMap(imageFile);
			file.Clear();
		}

	}
}

void ModelExporter::ExportMesh()
{
	ReadNode(m_scene->mRootNode, -1, -1);  // -1 : RootNode

	// Bone Tree구조 Print해보기
	/*
	for (auto a : nodes)
	{
		printf(" %d %d %s\n", a->parent, a->index, a->name.c_str());
	}
	*/

	ReadMesh(m_scene->mRootNode);
	WriteMesh();
}
// clipName : Dance,Run,Walk
void ModelExporter::ExportClip(string clipName)
{
	// fbx : Dance+Run+Walk 가 합쳐져 있는 경우도 고려
	for (UINT i = 0; i < m_scene->mNumAnimations; i++)
	{
		Clip* clip = ReadClip(m_scene->mAnimations[i]);
		WriterClip(clip, clipName, i);
	}
}
void ModelExporter::ReadMesh(aiNode* node)
{

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		MeshData* mesh = new MeshData();
		mesh->name = node->mName.C_Str();

		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = m_scene->mMeshes[index];

		mesh->matrialIndex = srcMesh->mMaterialIndex;

		UINT startVertex = mesh->vertices.size();

		vector<VertexWeights> vertexWeights(srcMesh->mNumVertices);

		ReadBone(srcMesh, vertexWeights);

		mesh->vertices.resize(srcMesh->mNumVertices);
		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			ModelVertex vertex;
			memcpy(&vertex.pos, &srcMesh->mVertices[v], sizeof(Float3));

			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Float2));

			if (srcMesh->HasNormals())
				memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Float3));

			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(Float3));

			if (!vertexWeights.empty())
			{
				vertexWeights[v].Normalize();

				vertex.indices.x = (float)vertexWeights[v].indices[0];
				vertex.indices.y = (float)vertexWeights[v].indices[1];
				vertex.indices.z = (float)vertexWeights[v].indices[2];
				vertex.indices.w = (float)vertexWeights[v].indices[3];

				vertex.weights.x = vertexWeights[v].weights[0];
				vertex.weights.y = vertexWeights[v].weights[1];
				vertex.weights.z = vertexWeights[v].weights[2];
				vertex.weights.w = vertexWeights[v].weights[3];
			}

			mesh->vertices[v] = vertex;
		}

		mesh->indices.resize(srcMesh->mNumFaces * 3);
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
			{
				mesh->indices[f * 3 + k] = face.mIndices[k] + startVertex;
			}
		}

		m_meshes.push_back(mesh);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadMesh(node->mChildren[i]);
}
void ModelExporter::ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights)
{
	for (UINT i = 0; i < mesh->mNumBones; i++)
	{
		UINT boneIndex = 0;
		string name = mesh->mBones[i]->mName.C_Str();

		if (m_boneMap.count(name) == 0)
		{
			boneIndex = m_boneCount++;
			m_boneMap[name] = boneIndex;

			BoneData* boneData = new BoneData();
			boneData->name = name;
			boneData->index = boneIndex;

			Matrix matrix(mesh->mBones[i]->mOffsetMatrix[0]);
			boneData->offset = XMMatrixTranspose(matrix);

			m_bones.push_back(boneData);
		}
		else
		{
			boneIndex = m_boneMap[name];
		}

		for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			UINT index = mesh->mBones[i]->mWeights[j].mVertexId;
			vertexWeights[index].Add(boneIndex, mesh->mBones[i]->mWeights[j].mWeight);
		}
	}
}

void ModelExporter::ReadNode(aiNode* node, int index, int parent)
{
	NodeData* nodeData = new NodeData();
	nodeData->index    = index;
	nodeData->parent   = parent;
	nodeData->name = node->mName.C_Str();

	Matrix matrix(node->mTransformation[0]);  // 0,1,--16
	nodeData->transform = XMMatrixTranspose(matrix);
	m_nodes.push_back(nodeData);

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ReadNode(node->mChildren[i], m_nodes.size(), index);
	}
}

void ModelExporter::WriteMesh()
{
	string path = "Assets/Meshes/" + m_name + "/" + m_name + ".mesh";
	
	StringPath::CreateFolders(path);

	BinaryWriter* writer = new BinaryWriter(path);

	// 메쉬크기
	writer->UInt(m_meshes.size());
	// 메쉬데이터 저장
	for (MeshData* mesh : m_meshes)
	{
		writer->String(mesh->name);
		writer->UInt(mesh->matrialIndex);

	//	printf("%s %d %d %d\n", mesh->name.c_str(), mesh->matrialIndex,
	//		mesh->vertices.size(), mesh->indices.size());

		writer->UInt(mesh->vertices.size());
		writer->Byte(mesh->vertices.data(), sizeof(ModelVertex) * mesh->vertices.size());

		writer->UInt(mesh->indices.size());
		writer->Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());

		delete mesh;
	}
	// meshes.clear() --> vector가 포인터형으로 생성된것이 없는 것
	m_meshes.erase(m_meshes.begin(), m_meshes.end());

	// Node저장
	{
		writer->UInt(m_nodes.size());
		for (NodeData* node : m_nodes)
		{
			writer->Int(node->index);
			writer->String(node->name);
			writer->Int(node->parent);
			writer->Matrix(node->transform);
			delete node;
		}
		m_nodes.erase(m_nodes.begin(), m_nodes.end());
	}

	// Bone저장
	{
		writer->UInt(m_bones.size());
		for (BoneData* bone : m_bones)
		{
			writer->Int(bone->index);
			writer->String(bone->name);
			writer->Matrix(bone->offset);
			delete bone;
		}
		m_bones.erase(m_bones.begin(), m_bones.end());
	}

	SAFE_DELETE(writer);
}

Clip* ModelExporter::ReadClip(aiAnimation* animation)
{
	Clip* clip = new Clip();

	clip->name = animation->mName.C_Str();
	clip->tickPerSecond = (float)animation->mTicksPerSecond;
	clip->frameCount    = (UINT)(animation->mDuration) + 1; // animation->mDuration : 12.123 일 때 반올림

	vector<ClipNode> clipNodes;
	clipNodes.reserve(animation->mNumChannels); // 애니메이션에 영향을 받는 Bone의 갯수 : 첫번째는 Head

	for (UINT i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* srcNode = animation->mChannels[i];
		ClipNode node;

		node.name = srcNode->mNodeName;

		// rotation(X), scaling(X)
		// Node마다 Scale, Rotation이 없는 경우가 있기 때문에 최대값으로 저장
		UINT keyCount = max(srcNode->mNumPositionKeys, srcNode->mNumRotationKeys);
		keyCount      = max(keyCount, srcNode->mNumScalingKeys);

		node.transforms.reserve(clip->frameCount);

		KeyTransform transform;

		for (UINT k = 0; k < keyCount; k++)
		{
			bool   isFound = false;
			float  t = node.transforms.size();

			if (k < srcNode->mNumPositionKeys &&
				GameMath::NearlyEqual((float)srcNode->mPositionKeys[k].mTime, t))
			{
				// Position이 있는 경우
				aiVectorKey key = srcNode->mPositionKeys[k];
				memcpy_s(&transform.pos, sizeof(Float3), &key.mValue, sizeof(aiVector3D));
				isFound = true;

			}
			if (k < srcNode->mNumRotationKeys &&
				GameMath::NearlyEqual((float)srcNode->mRotationKeys[k].mTime, t))
			{
				// Rotation이 있는 경우
				aiQuatKey key = srcNode->mRotationKeys[k];

				transform.rot.x = (float)key.mValue.x;
				transform.rot.y = (float)key.mValue.y;
				transform.rot.z = (float)key.mValue.z;
				transform.rot.w = (float)key.mValue.w;
				isFound = true;
			}
			if (k < srcNode->mNumScalingKeys &&
				GameMath::NearlyEqual((float)srcNode->mScalingKeys[k].mTime, t))
			{
				// Scale이 있는 경우
				aiVectorKey key = srcNode->mScalingKeys[k];
				memcpy_s(&transform.scale, sizeof(Float3), &key.mValue, sizeof(aiVector3D));
				isFound = true;
			}

			if (isFound)
				node.transforms.push_back(transform);
		}

		if (node.transforms.size() < clip->frameCount)
		{
			UINT count = clip->frameCount - node.transforms.size();
			KeyTransform keyTransform = node.transforms.back();

			for (UINT ii = 0; ii < count; ii++)
			{
				node.transforms.push_back(keyTransform);
			}
		}
		clipNodes.push_back(node);
	}

	ReadKeyFrame(clip, m_scene->mRootNode, clipNodes);

	return clip;
}

void ModelExporter::ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes)
{
	KeyFrame* keyFrame  = new KeyFrame();
	keyFrame->boneName = node->mName.C_Str();
	keyFrame->transforms.reserve(clip->frameCount);

	for (UINT i = 0; i < clip->frameCount; i++)
	{
		ClipNode* clipNode = nullptr;
		for(ClipNode& temp : clipNodes)
		{
			if (temp.name == node->mName)
			{
				clipNode = &temp;
				break;
			}
		}

		KeyTransform keytransform;
		if (clipNode == nullptr)
		{
			Matrix transform(node->mTransformation[0]);
			transform = XMMatrixTranspose(transform);

			Vector3 S, R, T;
			XMMatrixDecompose(S.GetValue(), R.GetValue(), T.GetValue(), transform);
			keytransform.scale = S;
			XMStoreFloat4(&keytransform.rot, R);
			keytransform.pos = T;
		}
		else
		{
			// clipNode가 없는 경우 현재 node의 transform으로 초기화
			keytransform = clipNode->transforms[i];
		}
		keyFrame->transforms.push_back(keytransform);
	}

	clip->KeyFrame.push_back(keyFrame);

	for (int i = 0; i < node->mNumChildren; i++)
		ReadKeyFrame(clip, node->mChildren[i], clipNodes);
}

void ModelExporter::WriterClip(Clip* clip, string clipName, UINT index)
{
	string path = "Assets/Clips/" + m_name + "/" +
		clipName + to_string(index) + ".clip";

	StringPath::CreateFolders(path);

	BinaryWriter writer(path);

	writer.String(clip->name);
	writer.UInt(clip->frameCount);
	writer.Float(clip->tickPerSecond);
	writer.UInt(clip->KeyFrame.size());

	for (KeyFrame* keyFrame : clip->KeyFrame)
	{
		writer.String(keyFrame->boneName);
		writer.UInt(keyFrame->transforms.size());
		writer.Byte(keyFrame->transforms.data(),
			sizeof(KeyTransform) * keyFrame->transforms.size());
		delete keyFrame;
	}

	clip->KeyFrame.erase(clip->KeyFrame.begin(), clip->KeyFrame.end());
	delete clip;

}

string ModelExporter::CreateTexture(string file)
{
	if (file.length() == 0)
		return "";

	string outName = StringPath::GetFileNameWithoutExtension(file) + ".png";
	string path     = "Assets/Textures/" + m_name + "/" + outName;

	StringPath::CreateFolders(path);

	const aiTexture* texture = m_scene->GetEmbeddedTexture(file.c_str());
	if (texture == nullptr)
		return "";

	if (texture->mHeight == 0)
	{
		BinaryWriter w(path);
		w.Byte(texture->pcData, texture->mWidth);
	}
	else
	{
		Image image;
		image.width = texture->mWidth;
		image.height = texture->mHeight;
		image.pixels = (uint8_t*)(texture->pcData);
		image.rowPitch = image.width * 4;
		image.slicePitch = image.width * image.height * 4;

		SaveToWICFile(image, WIC_FLAGS_NONE,
			GetWICCodec(WIC_CODEC_PNG), StringPath::ToWString(path).c_str());
	}

	return path;
}
