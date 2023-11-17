#include "framework.h"
#include "S02_Grid.h"

S02_Grid::S02_Grid()
{
	SetActive(true);

	m_material = new Material(L"Basic/Grid.hlsl");

	CreateMesh();
	m_worldBuffer = new MatrixBuffer();
}

S02_Grid::~S02_Grid()
{
	SAFE_DELETE(m_worldBuffer);
	SAFE_DELETE(m_mesh);
	SAFE_DELETE(m_material);
}

void S02_Grid::Update()
{

}

void S02_Grid::Render()
{
	m_worldBuffer->SetVS(0);

	m_material->Set();
	m_mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void S02_Grid::PreRender()
{
}

void S02_Grid::PostRender()
{
}

void S02_Grid::GUIRender()
{
}

void S02_Grid::CreateMesh()
{
	m_mesh = new Mesh<VertexColor>();
	vector<VertexColor>& vertices = m_mesh->GetVertices();

	// Xรเ
	vertices.emplace_back(-MAX_VALUE, 0, 0, 1, 0, 0);
	vertices.emplace_back(+MAX_VALUE, 0, 0, 1, 0, 0);

	// Yรเ
	vertices.emplace_back(0,-MAX_VALUE, 0, 0, 1, 0);
	vertices.emplace_back(0,+MAX_VALUE, 0, 0, 1, 0);

	// Zรเ
	vertices.emplace_back(0,0,-MAX_VALUE, 0, 0, 1);
	vertices.emplace_back(0,0,+MAX_VALUE, 0, 0, 1);

	int halfW = m_width  / 2;
	int halfH = m_height / 2;

	for (int x = -halfW; x <= halfW; x++)
	{
		if (x == 0) continue;

		vertices.emplace_back(x, 0, -halfH, 0.5f, 0.5f, 0.5f);
		vertices.emplace_back(x, 0, +halfH, 0.5f, 0.5f, 0.5f);
	}

	for (int z = -halfW; z <= halfW; z++)
	{
		if (z == 0) continue;

		vertices.emplace_back(-halfH, 0, z, 0.5f, 0.5f, 0.5f);
		vertices.emplace_back(+halfH, 0, z, 0.5f, 0.5f, 0.5f);
	}

	m_mesh->CreateMesh();
}

