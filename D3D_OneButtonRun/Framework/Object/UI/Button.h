#pragma once
class Button : public Quad
{
public:
	enum ButtonAction
	{
		CHANGESCENE = 0,
		QUIT,
		MAX,
	};

public:
	Button(wstring textureFile = L"Textures/Color/White.png", Vector2 size = Vector2(300.0f, 80.0f));
	~Button();

public:
	void Render() override;
	void GUIRender() override;

public:
	/// <summary>
	/// 2D Box X 좌표 충돌체크
	/// </summary>
	/// <param name="mousePos">마우스포지션</param>
	/// <returns>충돌여부 boolean 반환</returns>
	bool CheckMouseCollision(Vector3 mousePos);
	void OnClicked();
	/// <summary>
	/// 마우스 Hover시 이미지 바뀜 효과
	/// </summary>
	void OnHover(bool isHover);

public: // Getter
	bool GetPressability() { return m_pressable; }


public: //Setter
	/// <summary>
	/// 버튼 기능 설정 - QUIT
	/// </summary>
	/// <param name="actionType">기능 enum</param>
	void SetAction(ButtonAction actionType);
	/// <summary>
	/// 버튼 기능 설정 - CHANGESCENE, 바뀔 Scene이름 설정하기
	/// </summary>
	/// <param name="actionType">기능 enum</param>
	/// <param name="nextSceneName">바뀔 Scene이름</param>
	void SetAction(ButtonAction actionType, string nextSceneName);
	/// <summary>
	/// Hover시 버튼효과
	/// </summary>
	/// <param name="hoverTextureFile">오버랩할 이미지 파일경로</param>
	/// <param name="alpha">투명도 알파 값</param>
	void SetHoverEffct(wstring hoverTextureFile, float alpha);
	/// <summary>
	/// 클릭 시 이동 Scene 이름 설정
	/// </summary>
	/// <param name="sceneName"></param>
	void SetNextScene(string sceneName) { m_nextSceneName = sceneName; }
	/// <summary>
	/// 버튼 테두리 그리기
	/// </summary>
	/// <param name="margin">로컬스페이스 기준 테두리 굵기</param>
	/// <param name="colorTextureFile">테두리 텍스쳐이미지 파일</param>
	void SetOutline(float margin, wstring colorTextureFile = L"Textures/Color/White.png");
	/// <summary>
	/// 버튼 클릭기능 활성화
	/// </summary>
	/// <param name="isPressable">true / false 입력</param>
	void SetPressable(bool isPressable) { m_pressable = isPressable; }

private:
	// ActionType
	ButtonAction m_actionType = MAX;
	bool m_pressable = true;

	wstring m_buttonImage = L"";
	Texture* m_hoverTexture = nullptr;
	FloatValueBuffer* m_valueBuffer = nullptr;
	// wstring m_hoverImage = L"";
	string m_nextSceneName = "";

	// Hover
	float m_hoverAlpha = 0.0f;

	// Outline
	Texture* m_outlineTexture = nullptr;

};