#include   "framework.h"
#include   "Sphere.h"

Sphere::Sphere(wstring strShader, float radius, UINT sliceCount, UINT statckCount)
	: GameObject(strShader)
{
	m_radius = radius;
	m_sliceCount = sliceCount;
	m_stackCount = statckCount;

	m_mesh = new Mesh<VertexType>();

	MakeMesh();
	MakeTangent();

	m_mesh->CreateMesh();
}

Sphere::~Sphere()
{
	SAFE_DELETE(m_mesh);
}

void Sphere::Render()
{
	SetRender();

	m_mesh->Draw();
}

void Sphere::MakeMesh()
{
	float thetaStep = XM_2PI / m_sliceCount;
	float phiStep = XM_PI / m_stackCount;

	vector<VertexType>& vertices = m_mesh->GetVertices();
	vertices.reserve((m_sliceCount + 1) * (m_stackCount + 1));

	for (UINT i = 0; i <= m_stackCount; i++)
	{
		float phi = i * phiStep;
		for (UINT j = 0; j <= m_sliceCount; j++)
		{
			float theta = j * thetaStep;

			VertexType vertex;

			vertex.normal.x = sin(phi) * cos(theta);
			vertex.normal.y = cos(phi) ;
			vertex.normal.z = sin(phi) * sin(theta);

			vertex.pos = Vector3(vertex.normal) * m_radius;
			vertex.uv.x = (float)j / m_sliceCount;
			vertex.uv.y = (float)i / m_sliceCount;

			vertices.push_back(vertex);
		}
	}

	// index
	vector<UINT>& indices = m_mesh->GetIndices();

	indices.reserve(m_sliceCount * m_stackCount * 6);

	for (UINT i = 0; i < m_stackCount; i++)
	{
		for (UINT j = 0; j < m_sliceCount; j++)
		{
			indices.push_back((m_sliceCount + 1) * i + j);				//0
			indices.push_back((m_sliceCount + 1) * i + j + 1);			//2
			indices.push_back((m_sliceCount + 1) * (i + 1) + j);		//1

			indices.push_back((m_sliceCount + 1) * (i + 1) + j);		//1
			indices.push_back((m_sliceCount + 1) * i + j + 1);			//2
			indices.push_back((m_sliceCount + 1) * (i + 1) + j + 1);	//3
		}
	}
}

void Sphere::MakeTangent()
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
