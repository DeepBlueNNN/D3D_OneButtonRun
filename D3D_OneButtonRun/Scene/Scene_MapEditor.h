#pragma once

class Scene_MapEditor : public Scene
{
public:
	Scene_MapEditor();
	~Scene_MapEditor();

public:
	void Update()      override;
	void Render()      override;
	void PreRender()   override;
	void PostRender()  override;
	void GUIRender()   override;

};