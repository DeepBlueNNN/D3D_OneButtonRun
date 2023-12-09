#pragma once
class Scene_SelectStage : public Scene
{
public:
	Scene_SelectStage();
	~Scene_SelectStage();

public:
	void Update()      override;
	void Render()      override {}
	void PreRender()   override {}
	void PostRender()  override;
	void GUIRender()   override;

private:
	vector<string> m_stageNames;
	vector<wstring> m_thumbnails;
	vector<Button*> m_buttons;
};