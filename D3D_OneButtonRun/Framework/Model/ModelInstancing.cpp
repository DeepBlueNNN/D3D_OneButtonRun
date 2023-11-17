#include "framework.h"
#include "ModelInstancing.h"

ModelInstancing::ModelInstancing(string name)
	: Model(name)
{
	m_instanceBuffer = new VertexBuffer(m_instanceDatas, sizeof(InstanceData), MAX_INSTANCE);
	SetShader(L"Model/ModelInstancing.hlsl");
}

ModelInstancing::~ModelInstancing()
{
	SAFE_DELETE(m_instanceBuffer);

	for (Transform* transform : m_transforms)
		SAFE_DELETE(transform);
	m_transforms.erase(m_transforms.begin(), m_transforms.end());
}

void ModelInstancing::Update()
{
	m_drawCount = 0;
	for (UINT i = 0; i < m_transforms.size(); i++)
	{
		if (!m_transforms[i]->Active())
			continue;

		m_transforms[i]->UpdateWorld();
		m_instanceDatas[m_drawCount].world = XMMatrixTranspose(m_transforms[i]->GetWorld());
		m_instanceDatas[m_drawCount].index = i;
		m_drawCount++;
	}

	m_instanceBuffer->Update(m_instanceDatas, m_drawCount);
}

void ModelInstancing::Render()
{
	m_instanceBuffer->Set(1);

	for (ModelMesh* mesh : m_meshes)
		mesh->RenderInstanced(m_drawCount);
}

void ModelInstancing::GUIRender()
{
}

Transform* ModelInstancing::Add()
{
	Transform* transform = new Transform();
	transform->SetTag(m_name + "_" + to_string(m_transforms.size()));
	m_transforms.push_back(transform);

	return transform;
}
