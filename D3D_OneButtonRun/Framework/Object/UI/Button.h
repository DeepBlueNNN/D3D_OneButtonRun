#pragma once
class Button : public Quad
{
public:
	enum ButtonAction
	{
		CHANGESCENE = 0,
		QUIT,
		MAX
	};

public:
	Button(wstring textureFile = L"Textures/Color/White.png", Vector2 size = Vector2(300.0f, 80.0f));
	~Button();

public:
	void Update();
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
	/// Hover시 변경될 이미지 추가
	/// </summary>
	/// <param name="hoverTextureFile">이미지 파일경로</param>
	void SetHoverImage(wstring hoverTextureFile) { m_hoverImage = hoverTextureFile; }
	/// <summary>
	/// 클릭 시 이동 Scene 이름 설정
	/// </summary>
	/// <param name="sceneName"></param>
	void SetNextScene(string sceneName) { m_nextSceneName = sceneName; }
	/// <summary>
	/// 버튼 이미지 추가
	/// </summary>
	/// <param name="subTextureFile">이미지 파일경로</param>
	void SetSubImage(wstring subTextureFile);

private:
	ButtonAction m_actionType = MAX;
	Texture* m_subImage = nullptr;
	wstring m_buttonImage = L"";
	wstring m_hoverImage = L"";
	string m_nextSceneName = "";
};