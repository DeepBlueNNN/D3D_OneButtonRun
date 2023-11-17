#include "framework.h"
#include "SphereCollider.h"

SphereCollider::SphereCollider(float radius, UINT stackCount, UINT sliceCount)
{
	m_type = SPHERE;
	this->m_radius = radius;
	this->m_stackCount = stackCount;
	this->m_sliceCount = sliceCount;

    MakeMesh();

    m_mesh->CreateMesh();
}

// 벡터의 내적공식 3번 : || U + V ||^2 = ||U||^2 + 2U ● V + ||V||^2

// Ray공식 : Pos(P) + Direction(D)(방향-정규벡터) * Distance(t)
// 구의 중심 : C
// 
// 충돌조건
// 구 중심에서 Ray가 구에 닿는 지점까지 거리 벡터 구하기 : (P + D*t) - C = 결국 구의 반지름 r이어야 한다
// 
// |D*t + P-C|^2 = |r|*2  -> 우리가 아는 정보 P-C = a
//  = |D*t + a|^2 = |r|^2
//  = |D*t|^2 + 2D*t ● A + |A|^2 - r^2 = 0
// |D|^2 * t^2 + 2*D ● A * t + |A|^2 - r^2 = 0;
//  a * t^2 + 2b * t + c = 0 로 표현 할 예정 -> ax^2 + bx + c = 0 (근의공식 적용)
// 
//  a = |D|^2 = 1 (정규화된 두개의 같은 벡터를 내적하면 1이 나옴)
//  b = D ● A
//  c = |A|^2 - r^2
bool SphereCollider::IsRayCollision(Ray ray, Contact* contact)
{
    UpdateWorld();

    Vector3 P = ray.pos;
    Vector3 D = ray.dir.GetNormalized();

    Vector3 C = GlobalPos();
    Vector3 A = P - C;

    float k = Radius();
    float a = GameMath::Dot(D, D);
    float b = GameMath::Dot(D, A);
    float c = GameMath::Dot(A, A) - pow(Radius(),2);

    float k1 = GameMath::Dot(A, A);
    
    b = b * 2;
    if (pow(b,2) - 4 *a* c >= 0)
        {
            if (contact != nullptr)
            {
                float t = -b - sqrt(b * b - 4*a * c);
                contact->tag = m_tag;
                contact->distance = t;
                contact->hitPosition = P + D * t;

            }

            return true;
        }

	return false;
}

bool SphereCollider::IsBoxCollision(BoxCollider* collider)
{
	return collider->IsSphereCollision(this);
}

bool SphereCollider::IsSphereCollision(SphereCollider* collider)
{
    UpdateWorld();

    float distance = GameMath::Distance(GlobalPos(), collider->GlobalPos());
	return (distance <= (Radius() + collider->Radius()));
}

bool SphereCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
	return collider->IsSphereCollision(this);
}

void SphereCollider::MakeMesh()
{
    float thetaStep = XM_2PI / m_sliceCount;
    float phiStep = XM_PI / m_stackCount;

    vector<Vertex>& vertices = m_mesh->GetVertices();
    vertices.reserve((m_sliceCount + 1) * (m_stackCount + 1));

    for (UINT i = 0; i <= m_stackCount; i++)
    {
        float phi = i * phiStep;

        for (UINT j = 0; j <= m_sliceCount; j++)
        {
            float theta = j * thetaStep;

            Vertex vertex;
            vertex.pos.x = sin(phi) * cos(theta) * m_radius;
            vertex.pos.y = cos(phi) * m_radius;
            vertex.pos.z = sin(phi) * sin(theta) * m_radius;

            vertices.push_back(vertex);
        }
    }

    //Indices
    vector<UINT>& indices = m_mesh->GetIndices();

    indices.reserve(m_sliceCount * m_stackCount * 6);

    for (UINT i = 0; i < m_stackCount; i++)
    {
        for (UINT j = 0; j < m_sliceCount; j++)
        {
            indices.push_back((m_sliceCount + 1) * i + j);          //0
            indices.push_back((m_sliceCount + 1) * (i + 1) + j);    //1

            indices.push_back((m_sliceCount + 1) * i + j);          //0            
            indices.push_back((m_sliceCount + 1) * i + j + 1);      //2            
        }
    }
}
