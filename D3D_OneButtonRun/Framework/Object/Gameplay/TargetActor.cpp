#include "framework.h"
#include "TargetActor.h"

TargetActor::TargetActor()
	: GameActor(TARGET)
{
	string name = "crop_pumpkin";
	string file = "Assets/FBX/kenney_nature-kit/" + name + ".fbx";

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
}

void TargetActor::Render()
{
	if (!m_isActive) return;

	m_collider->Render();
	m_model->Render();
}

void TargetActor::GUIRender()
{
	if (ImGui::TreeNode((m_name + "_Detail").c_str()))
	{
		m_model->GUIRender();
		m_collider->GUIRender();

		ImGui::TreePop();
	}
}
