#pragma once
class SubMenu : public UIPanel
{
public:
	SubMenu();
	~SubMenu();

public:
	void Update() override;
	void Render() override;

private:
	// Textures
	vector<UITexture*> m_textures;

	// Buttons
	vector<Button*> m_buttons;
};