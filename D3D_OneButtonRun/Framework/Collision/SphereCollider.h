#pragma once
class SphereCollider : public Collider
{
public:
	SphereCollider(float radius = 1.0f, UINT stackCount =8, UINT sliceCount = 16);
	~SphereCollider() = default;

public:
    bool IsRayCollision(Ray ray, Contact* contact)     override;
    bool IsBoxCollision(BoxCollider* collider)         override;
    bool IsSphereCollision(SphereCollider* collider)   override;
    bool IsCapsuleCollision(CapsuleCollider* collider) override;

    Vector3& GetClosestPoint() override { return m_closestPoint; }

public:
    float   Radius() { return m_radius * max(max(GlobalScale().x, GlobalScale().y), GlobalScale().z); }

private:
    void MakeMesh() override;

private:
    float  m_radius = 1.0f;
    UINT   m_stackCount = 8;
    UINT   m_sliceCount = 16;
};
