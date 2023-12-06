#include "framework.h"
#include "S01_CubeMap.h"

S01_CubeMap::S01_CubeMap()
{
	m_sky = new Sky(L"Textures/CubeMap/skybox_winter.png");
}

S01_CubeMap::~S01_CubeMap()
{
}

void S01_CubeMap::Update()
{
}

void S01_CubeMap::Render()
{
	m_sky->Render();
}

void S01_CubeMap::PreRender()
{
}

void S01_CubeMap::PostRender()
{
}

void S01_CubeMap::GUIRender()
{
}
