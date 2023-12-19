#include "framework.h"
#include "GamePlayer.h"

GamePlayer::GamePlayer()
	: GameActor(PLAYER)
{
	/*string name = "Sparrow";
	string file = "Assets/FBX/" + name + ".fbx";

	ModelExporter* modelExporter = new ModelExporter(name, file);
	modelExporter->ExportMaterial();
	modelExporter->ExportMesh();
	delete(modelExporter);

	string clipName = "SparrowAnim";
	file = "Assets/Anim_FBX/" + name + "/" + clipName + ".fbx";

	modelExporter = new ModelExporter(name, file);
	modelExporter->ExportClip(clipName);
	delete(modelExporter);*/

	m_name = "Player";

	m_model = new ModelAnimator("Sparrow");
	dynamic_cast<ModelAnimator*>(m_model)->SetTag("Sparrow");
	m_collider = new SphereCollider();
	m_collider->SetTag("SphereCollider");

	for (UINT i = 0; i < 18; i++)
		dynamic_cast<ModelAnimator*>(m_model)->ReadClip("SparrowAnim", i);

	SetModelScale(Vector3(0.04f, 0.04f, 0.04f));
	//m_model->SetPivot(Vector3(0.0f, -1.3f, 0.0f));
	m_offset = 1.3f;
}

GamePlayer::~GamePlayer()
{
	SAFE_DELETE(m_collider);
	SAFE_DELETE(m_model);
}

void GamePlayer::Update()
{
	if (!m_isActive) return;

	if (KEY_DOWN('Q'))
	{
		dynamic_cast<ModelAnimator*>(m_model)->PlayClip(
			Random(0, dynamic_cast<ModelAnimator*>(m_model)->GetClipArraySize()));
	}

	if (m_isGravityActive)
	{
		m_velocity += GRAVITY * DELTA;
		SetPosition(GetColliderPosition() + m_velocity * DELTA);
		SetRotation(GetColliderRotation() + m_rotValue * 0.1f);
	}
	else
		SetRotation(GetColliderRotation());
		//SetRotation(Vector3(0.0f, 0.0f, 0.0f));

	m_collider->UpdateWorld();

	dynamic_cast<ModelAnimator*>(m_model)->Update();

	if (m_collidedObjects.size() > 0)
		m_isCollision = true;
	else
		m_isCollision = false;

}

void GamePlayer::Render()
{
	if (!m_isActive) return;

	m_collider->Render();
	dynamic_cast<ModelAnimator*>(m_model)->Render();
}

void GamePlayer::GUIRender()
{
	if (ImGui::TreeNode((m_name + "_Detail").c_str()))
	{
		dynamic_cast<ModelAnimator*>(m_model)->GUIRender();
		m_collider->GUIRender();

		ImGui::TreePop();
	}
}

void GamePlayer::Friction(Vector3 closestPoint)
{
	m_collider->UpdateWorld();

	Vector3 repulsionDir = (GetColliderPosition() - closestPoint).GetNormalized();
	float c = abs(Dot(m_velocity, repulsionDir));

	Vector3 prevVelocity = m_velocity;
	float repulsionValue = 0.9f;	// 0.5f 초과값으로 세팅
	float frictionValue  = 0.9f;	// 0 ~ 1 값으로 세팅 -> 0에 가까울 수록 마찰이 강하게 발생

	if (!m_isCollision)
	{
		m_velocity += (repulsionDir * c * 2.0f * repulsionValue);
	}
	else
	{
		frictionValue = 1.0f;
		Vector3 gravity = GRAVITY;
		Vector3 interpGravity= repulsionDir * (Dot(repulsionDir, gravity) * -1.0f) * DELTA;
		SetPosition(GetColliderPosition() + interpGravity);
	}
	
	Vector3 right   = Cross(prevVelocity, repulsionDir);
	Vector3 forward = Cross(repulsionDir, right).GetNormalized();
	m_velocity -= (forward * (1.0f - frictionValue) * Dot(prevVelocity, forward));

	right.Normalize();
	m_rotValue = (right * Dot(m_velocity, forward)) / (dynamic_cast<SphereCollider*>(m_collider)->Radius() * XM_2PI);
	system("cls");
	printf("%f %f %f \n", m_rotValue.x, m_rotValue.y, m_rotValue.z);
	m_rotValue.x = XMConvertToRadians(m_rotValue.x);
	m_rotValue.y = XMConvertToRadians(m_rotValue.y);
	m_rotValue.z = XMConvertToRadians(m_rotValue.z);
	m_rotValue *= -1.0f;
}

void GamePlayer::RegisterObject(Collider* object)
{
	m_collidedObjects.insert(object);
}

void GamePlayer::RemoveObject(Collider* object)
{
	if (m_collidedObjects.size() == 0)
		return;		// 집합 내부에 값이 없음

	if (m_collidedObjects.find(object) == m_collidedObjects.end())
		return;		// 집합 내부에 해당하는 Object가 존재하지 않음

	m_collidedObjects.erase(object);	// 해당하는 object를 삭제
}
