#pragma once

// DirectX �⺻ �浹ó�� Ŭ���� : BoundingBox > ����ȭ�� �ȵǾ����� �� ����

class BoxCollider;
class SphereCollider;
class CapsuleCollider;

struct Contact
{
	string	tag = "";
	Vector3	hitPosition;
	float	distance = 0.0f;
};

class Collider  : public GameObject
{
protected:
	enum Type
	{
		BOX, SPHERE, CAPSULE
	};

	const float PUSH_SPEED = 100.0f;

public:
	Collider();
	~Collider();

public:
	void Render();
	bool IsCollision(Collider* other);

public:
	virtual bool IsRayCollision(Ray ray, Contact* contact = nullptr) = 0;
	virtual bool IsBoxCollision(BoxCollider* collider) = 0;
	virtual bool IsSphereCollision(SphereCollider* collider) = 0;
	virtual bool IsCapsuleCollision(CapsuleCollider* collider) = 0;

public:
	void   SetColor(Float4 color) { m_material->GetData().diffuse = color; }
	void   SetColor(float r, float g, float b) { m_material->GetData().diffuse = { r,g,b,1 }; }
	bool   PushCollision(Collider* collider);

	static void RenderOnOff() { m_isRender = !m_isRender; } // Toggle

protected:
	virtual void MakeMesh() = 0;

protected:
	Type			m_type;
	Mesh<Vertex>*	m_mesh = nullptr;

private:
	static bool m_isRender;
};