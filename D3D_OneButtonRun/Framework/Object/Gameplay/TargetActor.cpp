#include "framework.h"
#include "TargetActor.h"

TargetActor::TargetActor()
	: GameActor(TARGET)
{
	string name = "crop_pumpkin";
	string file = "Assets/FBX/Nature/" + name + ".fbx";

	ModelExporter* modelExporter = new ModelExporter(name, file);
	modelExporter->ExportMaterial();
	modelExporter->ExportMesh();
	delete(modelExporter);

	m_name = "Target";
	m_model = new Model(name);
	m_model->SetTag(m_name);

	m_collider = new CapsuleCollider();
	m_collider->SetTag(m_name + "Collider");

	SetColliderScale(Vector3(2.0f, 0.7f, 2.0f));
	m_offset = 1.3f;

	m_spark = new Spark(L"Textures/Effect/Sparkle.png", true, true);
	m_spark->SetParticleCount(50);
	m_spark->SetRadiusRange(1.0f, 4.0f);
	m_spark->SetColor(Float4(255,255,0,255), Float4(255, 255, 0, 0));

	m_spark->Play(m_collider->Pos());
}

TargetActor::~TargetActor()
{
	SAFE_DELETE(m_model);
	SAFE_DELETE(m_collider);
}

void TargetActor::Update()
{
	if (!m_isActive) return;

	m_collider->UpdateWorld();

	m_model->Pos() = m_collider->Pos() + (m_collider->Down() * m_offset);

	m_model->Update();
	m_spark->Update(m_collider->Pos());
}

void TargetActor::Render()
{
	if (!m_isActive) return;

	m_collider->Render();
	m_model->Render();
	m_spark->Render();
}

void TargetActor::GUIRender()
{
	if (ImGui::TreeNode((m_name + "_Detail").c_str()))
	{
		m_model->GUIRender();
		m_collider->GUIRender();

		m_spark->GUIRender();

		ImGui::TreePop();

	}
}

void TargetActor::ParticleOn(bool isOn)
{
	m_spark->SetLoop(isOn);
}
