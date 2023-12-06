#include "framework.h"
#include "Button.h"

Button::Button(wstring textureFile, Vector2 size)
	:Quad(textureFile, size), m_buttonImage(textureFile)
{
	m_material->SetShader(L"Basic/Texture.hlsl");
}

Button::~Button()
{
}

void Button::Update()
{
}

void Button::Render()
{
	Quad::UpdateWorld();
	Quad::Render();
}

void Button::GUIRender()
{
	Quad::GUIRender();
}

bool Button::CheckMouseCollision(Vector3 mousePos)
{
	if (Pos().x - (m_size.x * 0.5f) < mousePos.x && mousePos.x < Pos().x + (m_size.x * 0.5f)
		&& Pos().y - (m_size.y * 0.5f) < mousePos.y && mousePos.y < Pos().y + (m_size.y * 0.5f))
	{
		return true;
	}

	return false;
}

void Button::OnClicked()
{
	switch (m_actionType)
	{
	case CHANGESCENE:
		if(m_nextSceneName == "MapEditor")
			SCENEMANAGER->ChangeScene(m_nextSceneName, true);
		else
			SCENEMANAGER->ChangeScene(m_nextSceneName);
		break;

	case QUIT:
		DestroyWindow(MAIN->GetWindowHandler());
		break;
	}
}

void Button::OnHover(bool isHover)
{
	if (isHover)
		m_material->SetDiffuseMap(m_hoverImage);
	else
		m_material->SetDiffuseMap(m_buttonImage);
}

void Button::SetAction(ButtonAction actionType)
{
	m_actionType = actionType;
}

void Button::SetAction(ButtonAction actionType, string nextSceneName)
{
	m_actionType = actionType;

	if (actionType == CHANGESCENE)
		SetNextScene(nextSceneName);
}

void Button::SetSubImage(wstring subTextureFile)
{
	m_subImage = Texture::Add(subTextureFile);
	m_subImage->PSSet(10);
}

