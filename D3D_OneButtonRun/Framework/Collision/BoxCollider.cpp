#include "framework.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(Vector3 size)
{
	m_type = BOX;
    m_size = size;

	MakeMesh();

	m_mesh->CreateMesh();
}

bool BoxCollider::IsRayCollision(Ray ray, Contact* contact)
{
	UpdateWorld();

    ObbDesc box;
    GetObb(box);

    Vector3 min = box.halfSize * -1.0f;
    Vector3 max = box.halfSize;

    // Box�� Ray�� ���� ����
    Vector3 delta = box.pos - ray.pos;

    // r.dir is unit direction vector of ray
    /*
    //dirfrac.x = 1.0f / r.dir.x;
    //dirfrac.y = 1.0f / r.dir.y;
    //dirfrac.z = 1.0f / r.dir.z;
    */
    Vector3 D = ray.dir.GetNormalized();

    float tMin = 0.0f;
    float tMax = FLT_MAX;

    // 3���� ��
    for (UINT i = 0; i < 3; i++)
    {
        Vector3 axis   = box.axis[i]; // ����ȭ�� ������
        float rayToBoxDis = GameMath::Dot(axis, delta);   // ��(����) * delta =  �� �࿡ �翵���� ������ ���� (������ ����)
        float slope = GameMath::Dot(D, axis);             // Ray�� �� ����� ���� cos��

        // Ray�� Box�� �� �� ���� �������� Check
        if (GameMath::NearlyEqual(abs(slope), 0.0f))
        {
            if (min[i] > rayToBoxDis || max[i] < rayToBoxDis) return false;
        }
        else
        {
            // ���� ���� Y = MX + C -> X = (Y - B) / M ����ȭ�� �´� ���� �����ؼ� ����
            float t1 = (rayToBoxDis - min[i]) / slope;
            float t2 = (rayToBoxDis - max[i]) / slope;

            if (t1 > t2)
                swap(t1, t2);   // Rotation�� �� ��� min���� max���� �ڹٲ����� ���� �ִ� - �׷� ���� �ڹٲ��ֱ�

            if (t2 < tMax) tMax = t2;
            if (t1 > tMin) tMin = t1;

            // �Ǻ���
            /*
                xmin, ymin�� �ִ밪�� xmin. (�̰��� max of min).
                xmax, ymax�� �ּҰ��� ymax. (�̰��� min of max)
                max of min > min of max ���� �浹 �� ��.
            */

            if (tMin > tMax) return false;
        }
    }

    if (contact != nullptr)
    {
        contact->tag = m_tag;
        contact->distance = tMin;
        // ���������� : pos = originPos + ���� * Ÿ������ -> Pos = O + D*T
        contact->hitPosition = ray.pos + ray.dir * tMin;
    }

	return true;
}

// 
bool BoxCollider::IsBoxCollision(BoxCollider* collider)
{
	UpdateWorld();

    ObbDesc box1;
    ObbDesc box2;

    // Box1�� Box2�� ��
    GetObb(box1);
    collider->GetObb(box2);

    Vector3 D = box2.pos - box1.pos;

    for (UINT i = 0; i < 3; i++)
    {
        if (IsSeperateAxis(D, box1.axis[i], box1, box2)) return false;
        if (IsSeperateAxis(D, box2.axis[i], box1, box2)) return false;
    }

    // ���� �࿡ ������
    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            if(GameMath::NearlyEqual(box1.axis[i], box2.axis[j]))
               return true;
        }
    }

    // ������ ���� �̿��� �Ǻ��ϴ� ���
    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            Vector3 cross = GameMath::Cross(box1.axis[i], box2.axis[j]);
            if (IsSeperateAxis(D, cross, box1, box2))
                return false;
        }
    }

	return true;
}

bool BoxCollider::IsSphereCollision(SphereCollider* collider)
{
	UpdateWorld();

    ObbDesc box;
    GetObb(box);

    // ���� ���� ������ Box�� ����Ʈ
    Vector3 closesetPointToSphere = box.pos;

    for (UINT i = 0; i < 3; i++)
    {
        Vector3 direction = collider->GlobalPos() - box.pos;

        float length = GameMath::Dot(box.axis[i], direction); // ����ȭ*�� = �Ÿ�
        float multi = (length < 0.0f) ? -1.0f : 1.0f;
        length = min(abs(length), box.halfSize[i]);

        closesetPointToSphere += box.axis[i] * length * multi;
    }

    float distance = GameMath::Distance(collider->GlobalPos(), closesetPointToSphere);

	return (distance <= collider->Radius());
}

bool BoxCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
	return collider->IsBoxCollision(this);
}

void BoxCollider::GetObb(ObbDesc& obbDesc)
{
    obbDesc.pos = GlobalPos();

    obbDesc.axis[0] = Right();
    obbDesc.axis[1] = Up();
    obbDesc.axis[2] = Forward();

    obbDesc.halfSize = m_size * 0.5f * GlobalScale();
}

void BoxCollider::MakeMesh()
{
    Vector3 halfSize = m_size * 0.5f;

    vector<Vertex>& vertices = m_mesh->GetVertices();

    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z);
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z);

    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z);
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z);

    vector<UINT>& indices = m_mesh->GetIndices();

    indices = {
        0, 1, 1, 3, 3, 2, 2, 0,
        4, 5, 5, 7, 7, 6, 6, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };
}

bool BoxCollider::IsSeperateAxis(Vector3 D, Vector3 axis, ObbDesc box1, ObbDesc box2)
{
    // Vector3 D = box2 ���� box1�� pos�� �� ��(�Ÿ�����) -> �࿡ �翵���� ����
    float  distance = abs(GameMath::Dot(D, axis));
    float  a = 0.0f;
    float  b = 0.0f;

    for (UINT i = 0; i < 3; i++)
    {
        Vector3 temp = box1.axis[i] * box1.halfSize[i];
        a += abs(GameMath::Dot(temp, axis));
        temp = box2.axis[i] * box2.halfSize[i];
        b += abs(GameMath::Dot(temp, axis));
    }

    // true : �������� ����
    return distance > (a+b);
}

