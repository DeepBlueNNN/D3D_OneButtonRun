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
	//SAFE_DELETE(m_hoverTexture);
	//SAFE_DELETE(m_outlineTexture);
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

/// <summary>
/// 2D Box X 좌표 충돌체크
/// </summary>
/// <param name="mousePos">마우스포지션</param>
/// <returns>충돌여부 boolean 반환</returns>
bool Button::CheckMouseCollision(Vector3 mousePos)
{
	if (Pos().x - (m_size.x * 0.5f) < mousePos.x && mousePos.x < Pos().x + (m_size.x * 0.5f)
		&& Pos().y - (m_size.y * 0.5f) < mousePos.y && mousePos.y < Pos().y + (m_size.y * 0.5f))
	{
		return true;
	}

	return false;
}

/// <summary>
/// 버튼 클릭 시 SetAction으로 설정한 버튼 동작기능 실행
/// </summary>
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

/// <summary>
/// 마우스 Hover시 이미지 바뀜 효과
/// </summary>
void Button::OnHover(bool isHover)
{
	if (isHover)
		m_valueBuffer->Get()[0] = m_hoverAlpha;
	else
		m_valueBuffer->Get()[0] = 0.0f;
}

/// <summary>
/// 버튼 기능 설정 - QUIT
/// </summary>
/// <param name="actionType">기능 enum</param>
void Button::SetAction(ButtonAction actionType)
{
	m_actionType = actionType;
}

/// <summary>
/// 버튼 기능 설정 - CHANGESCENE, 바뀔 Scene이름 설정하기
/// </summary>
/// <param name="actionType">기능 enum</param>
/// <param name="nextSceneName">바뀔 Scene이름</param>
void Button::SetAction(ButtonAction actionType, string nextSceneName)
{
	m_actionType = actionType;

	if (actionType == CHANGESCENE)
		SetNextScene(nextSceneName);
}

/// <summary>
/// Hover시 버튼효과
/// </summary>
/// <param name="hoverTextureFile">오버랩할 이미지 파일경로</param>
/// <param name="alpha">투명도 알파 값</param>
void Button::SetHoverEffct(wstring hoverTextureFile, float alpha)
{
	m_hoverTexture = Texture::Add(hoverTextureFile);
	m_hoverAlpha = alpha;
}

/// <summary>
/// 버튼 테두리 그리기
/// </summary>
/// <param name="margin">로컬스페이스 기준 테두리 굵기</param>
/// <param name="colorTextureFile">테두리 텍스쳐이미지 파일</param>
void Button::SetOutline(float margin, wstring colorTextureFile)
{
	m_valueBuffer->Get()[1] = margin;
	m_outlineTexture = Texture::Add(colorTextureFile);
}

