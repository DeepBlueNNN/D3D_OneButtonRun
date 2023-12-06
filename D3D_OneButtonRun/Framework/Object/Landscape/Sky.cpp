#include "framework.h"
#include "Sky.h"

Sky::Sky(wstring textureFile)
{
	wstring file = textureFile;
	m_sphere = new Sphere();
	m_sphere->GetMaterial()->SetShader(L"LandScape/Sky.hlsl");

	m_cubeMap = Texture::Add(file);

	m_rasterizerState[0] = new RasterizerState();
	m_rasterizerState[1] = new RasterizerState();
	m_rasterizerState[1]->FrontCounterClockwise(true);

	m_depthStencilState[0] = new DepthStencilState();
	m_depthStencilState[1] = new DepthStencilState();
	m_depthStencilState[1]->DepthEnable(false);
}

Sky::~Sky()
{
	SAFE_DELETE(m_sphere);
	SAFE_DELETE(m_rasterizerState[0]);
	SAFE_DELETE(m_rasterizerState[1]);
	SAFE_DELETE(m_depthStencilState[0]);
	SAFE_DELETE(m_depthStencilState[1]);
}

void Sky::Render()
{
	m_cubeMap->PSSet(10);
	m_rasterizerState[1]->SetState();
	m_depthStencilState[1]->SetState();

	m_sphere->Render();
	m_rasterizerState[0]->SetState();
	m_depthStencilState[0]->SetState();
}
