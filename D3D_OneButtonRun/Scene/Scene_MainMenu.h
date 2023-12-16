#pragma once

class Scene_MainMenu : public Scene
{
public:
	Scene_MainMenu();
	~Scene_MainMenu();

public:
	void Update()      override;
	void Render()      override;
	void PreRender()   override;
	void PostRender()  override;
	void GUIRender()   override;

private:
	vector<UITexture*> m_textures;
	vector<Button*> m_buttons;
};