#include "framework.h"
#include "InstancingActor.h"

InstancingActor::InstancingActor(GameActorTag actorType, string folder ,string fbxName)
	: GameActor(actorType)
{
	string file = "Assets/FBX/" + folder + "/" + fbxName + ".fbx";

	ModelExporter* modelExporter = new ModelExporter(fbxName, file);
	modelExporter->ExportMaterial();
	modelExporter->ExportMesh();
	delete(modelExporter);

	m_folder = folder;
	m_name = fbxName;
	m_model = new ModelInstancing(fbxName);
}

InstancingActor::~InstancingActor()
{
	for (auto& colliders : m_colliders)
	{
		for (auto& collider : colliders)
			SAFE_DELETE(collider);

		colliders.erase(colliders.begin(), colliders.end());
		colliders.clear();
	}
	m_colliders.erase(m_colliders.begin(), m_colliders.end());
	m_colliders.clear();
	SAFE_DELETE(m_model);
}

void InstancingActor::Update()
{
	GetModels()->Update();
	for (const auto& row : m_colliders)
	{
		for (const auto& collider : row)
		{
			collider->UpdateWorld();
		}
	}
}

void InstancingActor::Render()
{
	GetModels()->Render();
	for (const auto& row : m_colliders)
	{
		for (const auto& collider : row)
		{
			collider->Render();
		}
	}
}

void InstancingActor::GUIRender()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		for (UINT i = 0; i < GetModels()->GetTransforms().size(); i++)
		{
			string label = m_name + "_" + to_string(i + 1);
			if (ImGui::TreeNode(label.c_str()))
			{
				GetModels()->GetTransforms()[i]->GUIRender();
				for (const auto& collider : GetColliders()[i])
				{
					collider->GUIRender();
					collider->UpdateWorld();
				}

				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}

UINT InstancingActor::Add()
{
	// ModelInstancing->Add()
	Transform* transform = GetModels()->Add();
	transform->Pos() = Vector3(0, 0, 0);
	transform->Rot() = Vector3(0, 0, 0);
	transform->SetTag(m_name);

	// Add vector<Collider*>
	GetColliders().push_back(vector<Collider*>());

	return GetModels()->GetTransforms().size();
}

void InstancingActor::Erase(UINT index)
{
	if (GetModels()->GetTransforms().size() <= index) return;

	// erase actor's colliders
	for (auto& collider : m_colliders[index])
		SAFE_DELETE(collider);
	m_colliders[index].erase(m_colliders[index].begin(), m_colliders[index].end());

	// erase actor
	auto& transforms = GetModels()->GetTransforms();
	//SAFE_DELETE(transforms[index]);
	transforms.erase(remove(transforms.begin(), transforms.end(), transforms[index]), transforms.end());
}

UINT InstancingActor::AddCollider(UINT index, Collider::Type type)
{
	switch (type)
	{
	case Collider::BOX:
		GetColliders()[index].push_back(new BoxCollider());
		break;

	case Collider::SPHERE:
		GetColliders()[index].push_back(new SphereCollider());
		break;

	case Collider::CAPSULE:
		GetColliders()[index].push_back(new CapsuleCollider());
		break;
	}

	UINT id = GetColliders()[index].size();
	GetColliders()[index][id - 1]->Pos() = GetModels()->GetTransforms()[index]->Pos();
	GetColliders()[index][id - 1]->SetTag(to_string(id - 1));

	return GetColliders()[index].size();
}

void InstancingActor::EraseCollider(vector<Collider*>& colliders, Collider* collider)
{
	SAFE_DELETE(collider);

	colliders.erase(remove(colliders.begin(), colliders.end(), collider), colliders.end());
}

void InstancingActor::EraseCollider(vector<Collider*>& colliders, UINT index)
{
	SAFE_DELETE(colliders[index]);

	colliders.erase(remove(colliders.begin(), colliders.end(), colliders[index]), colliders.end());
}

void InstancingActor::EraseCollider(UINT index, Collider* collider)
{
	SAFE_DELETE(collider);

	m_colliders[index].erase(remove(m_colliders[index].begin(), m_colliders[index].end(), collider), m_colliders[index].end());
}

void InstancingActor::EraseCollider(UINT firstIndex, UINT secondIndex)
{
	SAFE_DELETE(m_colliders[firstIndex][secondIndex]);

	m_colliders[firstIndex].erase(remove(m_colliders[firstIndex].begin(), m_colliders[firstIndex].end(), 
		m_colliders[firstIndex][secondIndex]), m_colliders[firstIndex].end());
}
