#include "framework.h"
#include "UITexture.h"

UITexture::UITexture(wstring textureFile, Vector2 size)
	:Quad(textureFile, size), m_textureImage(textureFile)
{
	m_tag = "UITexture";
	m_material->SetShader(L"UI/UITexture.hlsl");

	m_valueBuffer = new FloatValueBuffer();
	m_valueBuffer->Get()[0] = 1.0f;
}

UITexture::~UITexture()
{
}

void UITexture::Render()
{
	m_valueBuffer->SetPS(10);
	Quad::UpdateWorld();
	Quad::Render();
}

void UITexture::GUIRender()
{
	Quad::GUIRender();
}

void UITexture::SetAlpha(float alpha)
{
	m_valueBuffer->Get()[0] = alpha;
}
