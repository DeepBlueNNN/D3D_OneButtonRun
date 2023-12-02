#include "framework.h"
#include "InstancingActor.h"

InstancingActor::InstancingActor(GameActorTag actorType, string fbxName, Collider::Type colliderType)
	: GameActor(actorType)
{
	string file = "Assets/FBX/" + fbxName + ".fbx";

	ModelExporter* modelExporter = new ModelExporter(fbxName, file);
	modelExporter->ExportMaterial();
	modelExporter->ExportMesh();
	delete(modelExporter);

	m_name = fbxName;
	m_model = new ModelInstancing(fbxName);
	m_colliderType = colliderType;
}

InstancingActor::~InstancingActor()
{
	SAFE_DELETE(m_model);
	m_colliders.erase(m_colliders.begin(), m_colliders.end());
	m_colliders.clear();
}

void InstancingActor::Update()
{
	GetModels()->Update();
	for (const auto collider : m_colliders)
		collider->UpdateWorld();
}

void InstancingActor::Render()
{
	GetModels()->Render();
	for (const auto collider : m_colliders)
		collider->Render();
}

void InstancingActor::GUIRender()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		for (int i = 0; i < m_colliders.size(); i++)
		{
			string label = m_name + "_" + to_string(i + 1);
			if (ImGui::TreeNode(label.c_str()))
			{
				GetModels()->GetTransforms()[i]->GUIRender();
				m_colliders[i]->GUIRender();

				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}

int InstancingActor::Add()
{
	// ModelInstancing->Add()
	int id = GetModels()->GetTransforms().size();
	Transform* transform = GetModels()->Add();
	transform->Pos() = Vector3(0, 0, 0);
	transform->Rot() = Vector3(0, 0, 0);
	transform->SetTag(m_name);

	// Add Collider
	switch (m_colliderType)
	{
	case Collider::BOX:
		m_colliders.push_back(new BoxCollider());
		break;

	case Collider::SPHERE:
		m_colliders.push_back(new SphereCollider());
		break;

	case Collider::CAPSULE:
		m_colliders.push_back(new CapsuleCollider());
		break;
	}

	return id;
}

void InstancingActor::Erase(int index)
{
	if (index >= m_colliders.size()) return;

	GetModels()->GetTransforms().erase(GetModels()->GetTransforms().begin() + index);
	m_colliders.erase(m_colliders.begin() + index);
}
