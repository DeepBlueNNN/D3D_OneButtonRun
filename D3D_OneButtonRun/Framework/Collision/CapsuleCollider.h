#pragma once
class CapsuleCollider : public Collider
{
private:
    struct TSphere
    {
        Vector3 center = Vector3(0.0f, 0.0f, 0.0f);
        float   radius = 0.0f;
    };

public:
	CapsuleCollider(float radius = 1.0f, float height = 2.0f, UINT stackCount = 8, UINT sliceCount = 16);
	~CapsuleCollider() = default;

public:
    bool IsRayCollision(Ray ray, Contact* contact)     override;
    bool IsBoxCollision(BoxCollider* collider)         override;
    bool IsSphereCollision(SphereCollider* collider)   override;
    bool IsCapsuleCollision(CapsuleCollider* collider) override;

private:
    void MakeMesh() override;

private:
    bool    IntersectRaySphere(Ray& ray, TSphere& sphere, float& tMin, float& tMax);
    bool    IsRayCollision2(Ray ray, Contact* contact);
    Vector3 ClosestPoint(Vector3 A, Vector3 B, Vector3 point);
    float   Clamp(float min, float max, float value);

public:
    float   Radius() { return m_radius * max(max(GlobalScale().x, GlobalScale().y), GlobalScale().z); }
    float   Height() { return m_height * GlobalScale().y; }

private:
    float   m_radius = 1.0f;
    float   m_height = 2.0f;
    UINT    m_stackCount = 8;
    UINT    m_sliceCount = 16;
};
