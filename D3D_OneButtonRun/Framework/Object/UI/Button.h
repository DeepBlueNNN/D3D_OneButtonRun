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
	bool CheckMouseCollision(Vector3 mousePos);
	void OnClicked();
	void OnHover(bool isHover);

public: // Getter
	bool GetPressability() { return m_pressable; }


public: //Setter
	void SetAction(ButtonAction actionType);
	void SetAction(ButtonAction actionType, string nextSceneName);
	void SetHoverEffct(wstring hoverTextureFile, float alpha);
	void SetOutline(float margin, wstring colorTextureFile = L"Textures/Color/White.png");
	/// <summary>
	/// ��ư Ŭ����� Ȱ��ȭ
	/// </summary>
	/// <param name="isPressable">true / false �Է�</param>
	void SetPressable(bool isPressable) { m_pressable = isPressable; }
	/// <summary>
	/// Ŭ�� �� �̵� Scene �̸� ����
	/// </summary>
	/// <param name="sceneName"></param>
	void SetNextScene(string sceneName) { m_nextSceneName = sceneName; }

private:
	// ActionType
	ButtonAction m_actionType = MAX;
	bool m_pressable = true;
	string m_nextSceneName = "";

	wstring m_buttonImage = L"";
	Texture* m_hoverTexture = nullptr;
	FloatValueBuffer* m_valueBuffer = nullptr;

	// Hover
	float m_hoverAlpha = 0.0f;

	// Outline
	Texture* m_outlineTexture = nullptr;

};