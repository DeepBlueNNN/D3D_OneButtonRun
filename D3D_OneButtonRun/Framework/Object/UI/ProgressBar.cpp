#include "framework.h"

ProgressBar::ProgressBar(wstring frontImageFile, wstring backImageFile)
	:Quad(frontImageFile)
{
	m_material->SetShader(L"UI/ProgressBar.hlsl");

	m_backImage = Texture::Add(backImageFile);

	m_valueBuffer = new FloatValueBuffer();
	m_valueBuffer->Get()[0] = m_fillAmount;
}

ProgressBar::~ProgressBar()
{
	SAFE_DELETE(m_valueBuffer);
}

void ProgressBar::Render()
{
	m_valueBuffer->SetPS(10);
	m_backImage->PSSet(10);

	Quad::UpdateWorld();
	Quad::Render();
}

void ProgressBar::SetAmount(float value)
{
	m_valueBuffer->Get()[0] = value;
}

void ProgressBar::GUIRender()
{
	Quad::GUIRender();
}
