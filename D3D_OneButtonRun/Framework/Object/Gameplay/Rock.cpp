#include "framework.h"
#include "Rock.h"

Rock::Rock(string fbxName, string id)
	:GameActor(ROCK)
{
	string file = "Assets/FBX/" + fbxName + ".fbx";

	ModelExporter* modelExporter = new ModelExporter(fbxName, file);
	modelExporter->ExportMaterial();
	modelExporter->ExportMesh();
	delete(modelExporter);

	m_name = fbxName + "_" + id;

	m_model = new Model(fbxName);
	m_model->SetTag(fbxName);
	m_collider = new SphereCollider();
	m_collider->SetTag("SphereCollider");

	SetModelScale(Vector3(0.35f, 0.35f, 0.35f));
	m_model->SetPivot(Vector3(0.0f, -1.0f, 0.0f));
}

Rock::~Rock()
{
	SAFE_DELETE(m_collider);
	SAFE_DELETE(m_model);
}

void Rock::Update()
{
	if (!m_isActive) return;

	m_collider->UpdateWorld();
	m_model->Update();
}

void Rock::Render()
{
	if (!m_isActive) return;

	m_collider->Render();
	m_model->Render();
}

void Rock::GUIRender()
{
	if (ImGui::TreeNode((m_name + "_Detail").c_str()))
	{
		m_model->GUIRender();
		m_collider->GUIRender();

		ImGui::TreePop();
	}
}
