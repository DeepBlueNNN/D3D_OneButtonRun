#include "framework.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"

bool Collider::m_isRender = true;

Collider::Collider() : GameObject(L"Basic/Collider.hlsl")
{
    m_mesh = new Mesh<Vertex>();
    SetColor(0, 1, 0);
}

Collider::~Collider()
{
    SAFE_DELETE(m_mesh);
}

void Collider::Render()
{
    if (!Active()) return;
    if (!m_isRender) return;

    SetRender();

    m_mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

bool Collider::IsCollision(Collider* collider)
{
    if (!Active()) return false;
    if(!collider->Active()) return false;
    bool  value = false;
    switch (collider->m_type)
    {
    case Collider::Type::BOX:
        value = IsBoxCollision((BoxCollider*)collider);
        break;

    case Collider::Type::SPHERE:
        value = IsSphereCollision((SphereCollider*)collider);
        break;

    case Collider::Type::CAPSULE:
        value = IsCapsuleCollision((CapsuleCollider*)collider);
        break;
    }
 
    return value;
}

bool Collider::PushCollision(Collider* collider)
{
    if (!IsCollision(collider)) return false;

    Vector3 dir = (collider->GlobalPos() - GlobalPos()).GetNormalized();

    collider->GetParent()->Pos() += dir * PUSH_SPEED * DELTA;

    return true;
}
