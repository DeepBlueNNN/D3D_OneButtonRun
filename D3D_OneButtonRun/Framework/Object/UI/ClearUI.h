#pragma once
class ClearUI : public UIPanel
{
public:
	ClearUI();
	~ClearUI();

public:
	void Update() override;
	void Render() override;

private:
	// Textures
	vector<UITexture*> m_textures;
};