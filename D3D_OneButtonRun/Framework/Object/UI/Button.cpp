#include "framework.h"
#include "Button.h"

Button::Button(wstring textureFile, Vector2 size)
	:Quad(textureFile, size), m_buttonImage(textureFile)
{
	m_tag = "Button";

	m_material->SetShader(L"UI/Button.hlsl");
	m_valueBuffer = new FloatValueBuffer();
}

Button::~Button()
{
	SAFE_DELETE(m_valueBuffer);
	SAFE_DELETE(m_hoverTexture);
	SAFE_DELETE(m_outlineTexture);
}

void Button::Render()
{
	m_valueBuffer->SetPS(10);
	m_hoverTexture->PSSet(20);

	Quad::UpdateWorld();
	Quad::Render();

	if (m_outlineTexture == nullptr)
		return;

	m_outlineTexture->PSSet(21);
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
	if (!m_pressable)
		return;

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

	// SceneChange직후 키보드상태 초기화
	KEYBOARD->Update();
}

void Button::OnHover(bool isHover)
{
	if (isHover)
		m_valueBuffer->Get()[0] = m_hoverAlpha;
	else
		m_valueBuffer->Get()[0] = 0.0f;
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

void Button::SetHoverEffct(wstring hoverTextureFile, float alpha)
{
	m_hoverTexture = Texture::Add(hoverTextureFile);
	m_hoverAlpha = alpha;
}

void Button::SetOutline(float margin, wstring colorTextureFile)
{
	m_valueBuffer->Get()[1] = margin;
	m_outlineTexture = Texture::Add(colorTextureFile);
}

