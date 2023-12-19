#include "framework.h"
#include "CapsuleCollider.h"
using namespace GameMath;

CapsuleCollider::CapsuleCollider(float radius, float height ,UINT stackCount, UINT sliceCount )
{
	m_type = CAPSULE;

	m_radius = radius;
	m_height = height;
	m_stackCount = stackCount;
	m_sliceCount = sliceCount;

	MakeMesh();

	m_mesh->CreateMesh();
}

bool CapsuleCollider::IsRayCollision(Ray ray, Contact* contact)
{
	// intersect capsule : https://iquilezles.org/articles/intersectors
	/*
	float capIntersect(in vec3 ro, in vec3 rd, in vec3 pa, in vec3 pb, in float r)
	{
		vec3  ba = pb - pa;
		vec3  oa = ro - pa;

		float baba = dot(ba, ba);
		float bard = dot(ba, rd);
		float baoa = dot(ba, oa);
		float rdoa = dot(rd, oa);
		float oaoa = dot(oa, oa);

		float a = baba - bard * bard;
		float b = baba * rdoa - baoa * bard;
		float c = baba * oaoa - baoa * baoa - r * r * baba;
		float h = b * b - a * c;
		if (h >= 0.0)
		{
			float t = (-b - sqrt(h)) / a;
			float y = baoa + t * bard;
			// body
			if (y > 0.0 && y < baba) return t;
			// caps
			vec3 oc = (y <= 0.0) ? oa : ro - pb;
			b = dot(rd, oc);
			c = dot(oc, oc) - r * r;
			h = b * b - c;
			if (h > 0.0) return -b - sqrt(h);
		}
		return -1.0;
	}
	return false;
	*/

	// ¿ø¸® : https://pastebin.com/2XrrNcxb
	/*
	Capsule is defined by A, B and r (A, B are points, r is the radius of the capsule)
	Ray is defined by it's origin O and the direction d.

	We want to find the two points, P1 and P2, which are the intersections of the ray with the capsule.
	Let's try to calculate one of them and call it P.

	Equations:
	1) P is on the ray (O, d)
	Px = Ox + t * dx (1)
	Py = Oy + t * dy (2)
	Pz = Oz + t * dz (3)

	2) A point K exists which is on the AB line for which the following is true:
	dot(AB, KP) = 0 (I)
	This point is on the AB line so the following holds true
	Kx = Ax + t' * ABx (4)
	Ky = Ay + t' * ABy (5)
	Kz = Az + t' * ABz (6)

	3) The magnitude of KP is r
	|KP| = r (II)

	Subtituting eq. (1) - (6) in (I) and (II) gives us two equations with two unknowns.
	Let's call them (I') and (II'). Solving (II') for (e.g.) t' and subtituting t' into (I') gives us two solutions for t.
	For each one of these solution we get a different t'.
	t1 and t2 correspond to intersection points P1 and P2 with the cylinder.
	t1' and t2' correspond to K1 and K2 on the AB line segment.

	If t1' and t2' are in the [0, 1] range the intersection points P1 and P2 are on the cylindrical part of the capsule.
	Otherwise they are invalid. So we check against the two spheres (A, r) and (B, r).

	Normals are the unit vectors KP if the intersection points are on the cylinder. Otherwise they can be calculated by the intesection point with the spheres and the corresponding center (A or B).

	Note that this doesn't take into account a ray parallel to the cylinder which intersects it at the two end spheres, but this should be easy.
	*/

    UpdateWorld();

	return IsRayCollision2(ray, contact);

	Vector3 aDirection = Up();  // ÇöÀç Ä¸½¶ÀÇ UpVector
	Vector3 pa = GlobalPos() - aDirection * Height() * 0.5f;
	Vector3 pb = GlobalPos() + aDirection * Height() * 0.5f;
	float   r = Radius();

	Vector3 ro = ray.pos;
	Vector3 rd = ray.dir;

	Vector3  ba = pb - pa;
	Vector3  oa = ro - pa;
    
    float baba = Dot(ba, ba);
    float bard = Dot(ba, rd);
    float baoa = Dot(ba, oa);
    float rdoa = Dot(rd, oa);
    float oaoa = Dot(oa, oa);
    
	// dot(Q, Q)*t^2 + 2*dot(Q, R)*t + (dot(R, R) - r^2) = 0
    float a = baba - bard * bard;
    float b = baba * rdoa - baoa * bard;
    float c = baba * oaoa - baoa * baoa - r * r * baba;
    float h = b * b - a * c;

    if (h >= 0.0)
    {
        float t = (-b - sqrt(h)) / a;
        float distance = baoa + t * bard;

        // Body
		if (distance > 0.0 && distance < baba)
		{
			if (contact)
			{
				contact->distance = distance;
				contact->hitPosition = ray.pos + ray.dir * t;
			}
			return true;
		}

        // Caps
        Vector3 oc = (distance <= 0.0) ? oa : ro - pb;
        b = Dot(rd, oc);
        c = Dot(oc, oc) - r * r;
        h = b * b - c;
		if (h > 0.0)
		{
			if (contact)
			{
				contact->distance = distance;
				contact->hitPosition = ray.pos + ray.dir * t;
			}
			return true;
		}	
    }
	return false;
}

bool CapsuleCollider::IsBoxCollision(BoxCollider* collider)
{
	UpdateWorld();

	BoxCollider::ObbDesc box;
	collider->GetObb(box);

	Vector3 Direction = Up();  // ÇöÀç Ä¸½¶ÀÇ UpVector
	Vector3 pa = GlobalPos() - Direction * Height() * 0.5f;
	Vector3 pb = GlobalPos() + Direction * Height() * 0.5f;

	Vector3 closestPointToSphere = box.pos;
	Vector3 pointOnLine = ClosestPoint(pa, pb, box.pos);

	for (UINT i = 0; i < 3; i++)
	{
		Vector3 direction = pointOnLine - box.pos;
		float   length = Dot(box.axis[i], direction);
		float   multi = (length < 0.0f) ? -1.0f : 1.0f;
		length = min(abs(length), box.halfSize[i]);
		closestPointToSphere += box.axis[i] * length * multi;
	}

	float distance = Distance(pointOnLine, closestPointToSphere);

	return (distance <= Radius());
}

bool CapsuleCollider::IsSphereCollision(SphereCollider* collider)
{
	UpdateWorld();

	Vector3 Direction = Up();  // ÇöÀç Ä¸½¶ÀÇ UpVector
	Vector3 pa = GlobalPos() - Direction * Height() * 0.5f;
	Vector3 pb = GlobalPos() + Direction * Height() * 0.5f;

	Vector3 P = collider->GlobalPos();
	Vector3 pointOnline = ClosestPoint(pa, pb, P);

	float  distance = Distance(P, pointOnline);
	m_closestPoint = pointOnline;
	return distance <= (Radius() + collider->Radius());
}

bool CapsuleCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
	UpdateWorld();

	Vector3 aDirection = Up();  // ÇöÀç Ä¸½¶ÀÇ UpVector
	Vector3 aA = GlobalPos() - aDirection * Height() * 0.5f;
	Vector3 aB = GlobalPos() + aDirection * Height() * 0.5f;

	Vector3 bDirection = collider->Up();  // ÇöÀç Ä¸½¶ÀÇ UpVector
	Vector3 bA = collider->GlobalPos() + bDirection * collider->Height() * 0.5f;
	Vector3 bB = collider->GlobalPos() - bDirection * collider->Height() * 0.5f;
	
	Vector3 V0 = bA - aA;
	Vector3 V1 = bB - aA;
	Vector3 V2 = bA - aB;
	Vector3 V3 = bB - aB;

	float  d0 = Dot(V0, V0);
	float  d1 = Dot(V1, V1);
	float  d2 = Dot(V2, V2);
	float  d3 = Dot(V3, V3);

	Vector3  bestA;
	if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1)
		bestA = aB;
	else
		bestA = aA;

	Vector3 bestB = ClosestPoint(bA, bB, bestA);
	bestA = ClosestPoint(aA, aB, bestB);
	bestB = ClosestPoint(bA, bB, bestA);

	float distance = Distance(bestA, bestB);

	return distance <= (Radius() + collider->Radius());
}

void CapsuleCollider::MakeMesh()
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

            if (vertex.pos.y > 0)
                vertex.pos.y += m_height * 0.5f;
            else if (vertex.pos.y < 0)
                vertex.pos.y -= m_height * 0.5f;

            vertices.push_back(vertex);
        }
    }

    //Indices
    vector<UINT>& indices = m_mesh->GetIndices();

    indices.reserve((size_t)(m_sliceCount * m_stackCount * 6));

    for (UINT i = 0; i < m_stackCount; i++)
    {
        for (UINT j = 0; j < m_sliceCount; j++)
        {
            indices.push_back((m_sliceCount + 1) * i + j);			//0
            indices.push_back((m_sliceCount + 1) * (i + 1) + j);	//1

            indices.push_back((m_sliceCount + 1) * i + j);			//0            
            indices.push_back((m_sliceCount + 1) * i + j + 1);		//2            
        }
    }
}

bool CapsuleCollider::IntersectRaySphere(Ray& ray, TSphere& sphere, float& tMin, float& tMax)
{
    Vector3 CO = ray.pos - sphere.center;

    float a = GameMath::Dot(ray.dir, ray.dir);
    float b = 2.0f * GameMath::Dot(CO, ray.dir);
    float c = GameMath::Dot(CO, CO) - (sphere.radius * sphere.radius);

    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
        return false;

    tMin = (-b - sqrtf(discriminant)) / (2.0f * a);
    tMax = (-b + sqrtf(discriminant)) / (2.0f * a);
    if (tMin > tMax)
    {
        float temp = tMin;
        tMin = tMax;
        tMax = temp;
    }
    return true;
}

bool CapsuleCollider::IsRayCollision2(Ray ray, Contact* contact)
{
    // Substituting equ. (1) - (6) to equ. (I) and solving for t' gives:
    //
    // t' = (t * dot(AB, d) + dot(AB, AO)) / dot(AB, AB); (7) or
    // t' = t * m + n where 
    // m = dot(AB, d)   / dot(AB, AB) and 
    // n = dot(AB, AO) / dot(AB, AB)

    Vector3 aDirection = Up();  // ÇöÀç Ä¸½¶ÀÇ UpVector
    Vector3 cA = GlobalPos() - aDirection * Height() * 0.5f;
    Vector3 cB = GlobalPos() + aDirection * Height() * 0.5f;

    Vector3 AB = cB - cA;
    Vector3 AO = ray.pos - cA;

    float AB_dot_d = Dot(AB, ray.dir);
    float AB_dot_AO = Dot(AB, AO);
    float AB_dot_AB = Dot(AB, AB);

    float m = AB_dot_d / AB_dot_AB;
    float n = AB_dot_AO / AB_dot_AB;

    // Substituting (7) into (II) and solving for t gives:
    //
    // dot(Q, Q)*t^2 + 2*dot(Q, R)*t + (dot(R, R) - r^2) = 0
    // where
    // Q = d - AB * m
    // R = AO - AB * n
	Vector3 Q = ray.dir - (AB * m);
	Vector3 R = AO - (AB * n);

	float a = Dot(Q, Q);
	float b = 2.0f * Dot(Q, R);
	float c = Dot(R, R) - (Radius() * Radius());

	if (a == 0.0f)
	{
		// Special case: AB and ray direction are parallel. If there is an intersection it will be on the end spheres...
		// NOTE: Why is that?
		// Q = d - AB * m =>
		// Q = d - AB * (|AB|*|d|*cos(AB,d) / |AB|^2) => |d| == 1.0
		// Q = d - AB * (|AB|*cos(AB,d)/|AB|^2) =>
		// Q = d - AB * cos(AB, d) / |AB| =>
		// Q = d - unit(AB) * cos(AB, d)
		//
		// |Q| == 0 means Q = (0, 0, 0) or d = unit(AB) * cos(AB,d)
		// both d and unit(AB) are unit vectors, so cos(AB, d) = 1 => AB and d are parallel.
		// 
		TSphere sphereA, sphereB;
		sphereA.center = cA;
		sphereA.radius = Radius();
		sphereB.center = cB;
		sphereB.radius = Radius();

		float atmin, atmax, btmin, btmax;
		if (!IntersectRaySphere(ray, sphereA, atmin, atmax) ||
			!IntersectRaySphere(ray, sphereB, btmin, btmax))
		{
			// No intersection with one of the spheres means no intersection at all...
			return false;
		}

		if (atmin < btmin)
		{
			if (contact)
			{
				contact->hitPosition = ray.pos + ray.dir * atmin;
				contact->distance = Distance(ray.pos, contact->hitPosition);
			}
		}
		else
		{
			if (contact)
			{
				contact->hitPosition = ray.pos + ray.dir * btmin;
				contact->distance = Distance(ray.pos, contact->hitPosition);
			}
		}

		if (atmax > btmax)
		{
			if (contact)
			{
				contact->hitPosition = ray.pos + ray.dir * atmax;
				contact->distance = Distance(ray.pos, contact->hitPosition);
			}
		}
		else
		{
			if (contact)
			{
				contact->hitPosition = ray.pos + ray.dir * btmax;
				contact->distance = Distance(ray.pos, contact->hitPosition);
			}
		}

		return true;
	}

	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
	{
		// The ray doesn't hit the infinite cylinder defined by (A, B).
		// No intersection.
		return false;
	}

	float tmin = (-b - sqrtf(discriminant)) / (2.0f * a);
	float tmax = (-b + sqrtf(discriminant)) / (2.0f * a);
	if (tmin > tmax)
	{
		float temp = tmin;
		tmin = tmax;
		tmax = temp;
	}

	// Now check to see if K1 and K2 are inside the line segment defined by A,B
	float t_k1 = tmin * m + n;
	if (t_k1 < 0.0f)
	{
		// On sphere (A, r)...
		TSphere s;
		s.center = cA;
		s.radius = Radius();

		float stmin, stmax;
		if (IntersectRaySphere(ray, s, stmin, stmax))
		{
			if (contact)
			{
				contact->hitPosition = ray.pos + ray.dir * stmin;
				contact->distance = Distance(ray.pos, contact->hitPosition);
			}
		}
		else
			return false;
	}
	else if (t_k1 > 1.0f)
	{
		// On sphere (B, r)...
		TSphere s;
		s.center = cB;
		s.radius = Radius();

		float stmin, stmax;
		if (IntersectRaySphere(ray, s, stmin, stmax))
		{
			if (contact)
			{
				contact->hitPosition = ray.pos + ray.dir * stmin;
				contact->distance = Distance(ray.pos, contact->hitPosition);
			}
		}
		else
			return false;
	}
	else
	{
		// On the cylinder...
		if (contact)
		{
			contact->hitPosition = ray.pos + ray.dir * tmin;
			contact->distance = Distance(ray.pos, contact->hitPosition);
		}
	}

	float t_k2 = tmax * m + n;
	if (t_k2 < 0.0f)
	{
		// On sphere (A, r)...
		TSphere s;
		s.center = cA;
		s.radius = Radius();

		float stmin, stmax;
		if (IntersectRaySphere(ray, s, stmin, stmax))
		{
			if (contact)
			{
				contact->hitPosition = ray.pos + ray.dir * stmax;
				contact->distance = Distance(ray.pos, contact->hitPosition);
			}
		}
		else
			return false;
	}
	else if (t_k2 > 1.0f)
	{
		// On sphere (B, r)...
		TSphere s;
		s.center = cB;
		s.radius = Radius();

		float stmin, stmax;
		if (IntersectRaySphere(ray, s, stmin, stmax))
		{
			if (contact)
			{
				contact->hitPosition = ray.pos + ray.dir * stmax;
				contact->distance = Distance(ray.pos, contact->hitPosition);
			}
		}
		else
			return false;
	}
	else
	{
		if (contact)
		{
			contact->hitPosition = ray.pos + ray.dir * tmax;
			contact->distance = Distance(ray.pos, contact->hitPosition);
		}
	}

	return true;
}
// S-> E  = L
// S-> P  = A
// L ¡Ü A  = t * |L|
// X/Y = t * |L| / |L|*|L|
// X/Y = t / |L|
Vector3 CapsuleCollider::ClosestPoint(Vector3 S, Vector3 E, Vector3 point)
{
	Vector3 L = E     - S;
	Vector3 A = point - S;
	float x = Dot(L, A);
	float y = Dot(L, L);  
	float t = Clamp(0.0f, 1.0f, x / y);

	return S + L*t;
}

float CapsuleCollider::Clamp(float min, float max, float value)
{
	if(value  <  min)
		value =  min;
	if (value > max)
		value = max;

	return value;
}
