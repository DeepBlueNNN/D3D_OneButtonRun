#pragma once
class BoxCollider : public Collider
{
public:
    struct ObbDesc
    {
        Vector3  pos;
        Vector3  axis[3];
        Vector3  halfSize;
    };

public:
	BoxCollider(Vector3 size = Vector3(1.0f, 1.0f, 1.0f));
	~BoxCollider() = default;

public:
    bool IsRayCollision(Ray ray, Contact* contact = nullptr) override;
    bool IsBoxCollision(BoxCollider* collider) override;
    bool IsSphereCollision(SphereCollider* collider) override;
    bool IsCapsuleCollision(CapsuleCollider* collider) override;

    void GetObb(ObbDesc& obbDesc);

    Vector3& GetClosestPoint() { return m_closestPoint; }

private:
    void MakeMesh() override;

private:
    bool  IsSeperateAxis(Vector3 D, Vector3 axix, ObbDesc box1, ObbDesc box2);

private:
    Vector3 m_size = Vector3(1.0f, 1.0f, 1.0f);
    Vector3 m_closestPoint = Vector3::Zero();
};

////////////////////////////////////////////////////
// Box x Ray Intersection
// : Slab Theory
//  https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=ybill&logNo=120103496030
//  https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
//  https://education.siggraph.org/static/HyperGraph/raytrace/rtinter3.htm
// 좀더 쉬움?! https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
////////////////////////////////////////////////////