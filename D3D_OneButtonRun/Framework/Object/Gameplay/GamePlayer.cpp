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

	m_model = new ModelAnimator("Sparrow");
	m_collider = new SphereCollider();

	for (UINT i = 0; i < 18; i++)
		dynamic_cast<ModelAnimator*>(m_model)->ReadClip("SparrowAnim", i);

	SetModelScale(Vector3(0.04f, 0.04f, 0.04f));
	m_model->SetPivot(Vector3(0.0f, -1.3f, 0.0f));

	curPos = GetColliderPosition();
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

//	prevPos = curPos;
//	curPos = GetColliderPosition();
//	velocity = (curPos - prevPos) / DELTA;
	velocity += GRAVITY * DELTA;
	SetPosition(GetColliderPosition() + velocity * DELTA);

	m_collider->UpdateWorld();

	dynamic_cast<ModelAnimator*>(m_model)->Update();

}

void GamePlayer::Render()
{
	if (!m_isActive) return;

	m_collider->Render();
	dynamic_cast<ModelAnimator*>(m_model)->Render();
}

void GamePlayer::GUIRender()
{
	m_collider->GUIRender();
	dynamic_cast<ModelAnimator*>(m_model)->GUIRender();
}
