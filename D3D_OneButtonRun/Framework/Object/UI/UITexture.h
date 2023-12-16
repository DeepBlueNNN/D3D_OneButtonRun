#pragma once
class UITexture : public Quad
{
public:
	UITexture(wstring textureFile = L"Textures/Color/White.png", Vector2 size = Vector2(300.0f, 80.0f));
	~UITexture();

public:
	void Render() override;
	void GUIRender() override;
	void SetAlpha(float alpha);

private:
	wstring m_textureImage = L"";
	FloatValueBuffer* m_valueBuffer = nullptr;

};