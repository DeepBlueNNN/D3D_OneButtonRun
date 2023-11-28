#include "framework.h"
#include "Scene_MapEditor.h"

Scene_MapEditor::Scene_MapEditor()
{
	vector<GameActor*>& actors = SAVELOAD->GetGameActors();

	GameActor* tree2 = new Tree("Tree_01", "1");
//	tree2->SetPosition(Vector3(-10, 3, 0));
	actors.push_back(tree2);

	GameActor* tree = new Tree("Tree_02","1");
//	tree->SetPosition(Vector3(0, 3, 0));
	actors.push_back(tree);

	GameActor* rock = new Rock("Rock_01", "1");
//	rock->SetPosition(Vector3(-5, 1, 0));
	actors.push_back(rock);
}

Scene_MapEditor::~Scene_MapEditor()
{
}

void Scene_MapEditor::Update()
{
	vector<GameActor*>& actors = SAVELOAD->GetGameActors();
	for (const auto actor : actors)
		actor->Update();
}

void Scene_MapEditor::Render()
{
	vector<GameActor*>& actors = SAVELOAD->GetGameActors();
	for (const auto actor : actors)
		actor->Render();
}

void Scene_MapEditor::PreRender()
{
}

void Scene_MapEditor::PostRender()
{
}

void Scene_MapEditor::GUIRender()
{
	vector<GameActor*>& actors = SAVELOAD->GetGameActors();
	for (const auto actor : actors)
		actor->GUIRender();

	SAVELOAD->GUIRender();
}
