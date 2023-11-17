#include   "framework.h"
#include   "Cylinder.h"

Cylinder::Cylinder(wstring strShader, float radius, float height, UINT sliceCount)
	: GameObject(strShader)
{
	m_tag = "Cylinder";
	m_radius     = radius;
	m_sliceCount = sliceCount;
	m_height     = height;

	m_mesh = new Mesh<VertexType>();

	MakeMesh();
	MakeNormal();
	MakeTangent();

	m_mesh->CreateMesh();
}

Cylinder::~Cylinder()
{
	SAFE_DELETE(m_mesh);
}

void Cylinder::Render()
{
	SetRender();

	m_mesh->Draw();
}

void Cylinder::MakeMesh()
{
	float  thetaStep = XM_2PI / m_sliceCount;

	vector<VertexType>& vertices = m_mesh->GetVertices();
	vertices.reserve((m_sliceCount + 1) * 4);

	VertexType top;
	top.pos = { 0.0f, m_height * 0.5f,0.0f };
	top.uv = { 0.5f, 0.5f };

    // Top Side
	vertices.push_back(top);
	for (int i = 0; i <= m_sliceCount; i++)
	{
		float theta = thetaStep * i;
		float x = cos(theta);
		float z = sin(theta);

		VertexType vertex;
		vertex.pos = { x * m_radius,m_height * 0.5f,z * m_radius };
		vertex.uv = { (x + 1) / 2, (z + 1) / 2 };

		vertices.push_back(vertex);
	}

	// Bottom Side
	VertexType bottom;
	bottom.pos = { 0.0f, -m_height * 0.5f,0.0f };
	bottom.uv = { 0.5f, 0.5f };

	vertices.push_back(bottom);

	for (int i = 0; i <= m_sliceCount; i++)
	{
		float theta = thetaStep * i;
		float x = cos(theta);
		float z = sin(theta);

		VertexType vertex;
		vertex.pos = { x * m_radius,-m_height * 0.5f,z * m_radius };
		vertex.uv = { (x + 1) / 2, (z + 1) / 2 };

		vertices.push_back(vertex);
	}

	// Side
	for (int i = 0; i <= m_sliceCount; i++)
	{
		float theta = thetaStep * i;
		float x = cos(theta);
		float z = sin(theta);

		VertexType vertex;
		vertex.pos = { x * m_radius,-m_height * 0.5f,z * m_radius };
		vertex.uv = { (float)i/(float)m_sliceCount, 1.0f};

		vertices.push_back(vertex);

		vertex.pos = { x * m_radius,+m_height * 0.5f,z * m_radius };
		vertex.uv = { (float)i / (float)m_sliceCount, 0.0f };

		vertices.push_back(vertex);
	}

	// Index
	vector<UINT>& indices = m_mesh->GetIndices();
	indices.reserve(m_sliceCount * 6);

	// Topside index
	for (UINT i = 0; i < m_sliceCount; i++)
	{
	   indices.push_back(0);        
	   indices.push_back(i+2);   
	   indices.push_back(i+1);
	}

	// Bottom side index
	UINT bottomIndex = m_sliceCount + 2;
	for (UINT i = 0; i < m_sliceCount; i++)
	{
		indices.push_back(bottomIndex);
		indices.push_back(bottomIndex+i + 2);
		indices.push_back(bottomIndex+i + 1);
	}

	// Side index
	UINT sideIndex = (m_sliceCount + 1) * 2 + 2;
	for (UINT i = 0; i < m_sliceCount; i++)
	{
		indices.push_back(sideIndex + i *2 + 0);  // 0
		indices.push_back(sideIndex + i *2 + 1);  // 1
		indices.push_back(sideIndex + i *2 + 2);  // 2

		indices.push_back(sideIndex + i * 2 + 2);  // 2
		indices.push_back(sideIndex + i * 2 + 1);  // 1
		indices.push_back(sideIndex + i * 2 + 3);  // 3
	}
}

void Cylinder::MakeNormal()
{
	vector<VertexType>& vertices = m_mesh->GetVertices();	// 좌표점
	vector<UINT>& indices = m_mesh->GetIndices();			// 3점을 이루는 index

	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].pos;
		Vector3 v1 = vertices[index1].pos;
		Vector3 v2 = vertices[index2].pos;

		// 백터의 외적을 이용하여 면에 수직되는 법선(Normal)벡터를 만듬
		Vector3 e0 = v1 - v0;
		Vector3 e1 = v2 - v0;

		Vector3 noraml = GameMath::Cross(e0, e1).GetNormalized();

		vertices[index0].normal = noraml + vertices[index0].normal;
		vertices[index1].normal = noraml + vertices[index1].normal;
		vertices[index2].normal = noraml + vertices[index2].normal;
	}
}

void Cylinder::MakeTangent()
{
	vector<VertexType>& vertices = m_mesh->GetVertices();
	vector<UINT>& indices = m_mesh->GetIndices();

	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		//정점의 index값
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		// index를 이용하여 3의 정점 가져오기
		Vector3 p0 = vertices[index0].pos;
		Vector3 p1 = vertices[index1].pos;
		Vector3 p2 = vertices[index2].pos;

		// UV좌표값 가져오기 --> 노말ㄹ맵의 픽셀값아님
		Vector2 uv0 = vertices[index0].uv;
		Vector2 uv1 = vertices[index1].uv;
		Vector2 uv2 = vertices[index2].uv;

		// 방향벡터
		//   p0->p1벡터
		Vector3 e0 = p1 - p0;
		//   p0->p2벡터
		Vector3 e1 = p2 - p0;

		/*
			E0 = (u1-u0)*T + (v1-v0)*B
			E1 = (u2-u1)*T + (v2-v1)*B
			
			| E0 |     | U1    V1 |    |  T  |
			|    | =   |          |    |     |
			| E1 |     | U2    V2 |    |  B  |
			
			역행렬을 취해서 계산
			T = d*(V2*e0 - V1*e1)
		*/
		float U1 = uv1.x - uv0.x;
		float V1 = uv1.y - uv0.y;
		float U2 = uv2.x - uv0.x;
		float V2 = uv2.y - uv0.y;

		float d = 1.0f / (U1 * V2 - V1 * U2);
		Vector3 tangent = d * (e0 * V2 - e1 * V1);

		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}
}
