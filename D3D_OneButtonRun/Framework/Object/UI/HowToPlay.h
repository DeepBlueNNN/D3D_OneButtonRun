#pragma once
class HowToPlay : public UIPanel
{
public:
	HowToPlay();
	~HowToPlay();

public:
	void Update() override;
	void Render() override;

private:
	// Textures
	vector<UITexture*> m_textures;

};