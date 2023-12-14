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
	/// 2D Box X ��ǥ �浹üũ
	/// </summary>
	/// <param name="mousePos">���콺������</param>
	/// <returns>�浹���� boolean ��ȯ</returns>
	bool CheckMouseCollision(Vector3 mousePos);
	void OnClicked();
	/// <summary>
	/// ���콺 Hover�� �̹��� �ٲ� ȿ��
	/// </summary>
	void OnHover(bool isHover);

public: // Getter
	bool GetPressability() { return m_pressable; }


public: //Setter
	/// <summary>
	/// ��ư ��� ���� - QUIT
	/// </summary>
	/// <param name="actionType">��� enum</param>
	void SetAction(ButtonAction actionType);
	/// <summary>
	/// ��ư ��� ���� - CHANGESCENE, �ٲ� Scene�̸� �����ϱ�
	/// </summary>
	/// <param name="actionType">��� enum</param>
	/// <param name="nextSceneName">�ٲ� Scene�̸�</param>
	void SetAction(ButtonAction actionType, string nextSceneName);
	/// <summary>
	/// Hover�� ��ưȿ��
	/// </summary>
	/// <param name="hoverTextureFile">�������� �̹��� ���ϰ��</param>
	/// <param name="alpha">���� ���� ��</param>
	void SetHoverEffct(wstring hoverTextureFile, float alpha);
	/// <summary>
	/// Ŭ�� �� �̵� Scene �̸� ����
	/// </summary>
	/// <param name="sceneName"></param>
	void SetNextScene(string sceneName) { m_nextSceneName = sceneName; }
	/// <summary>
	/// ��ư �׵θ� �׸���
	/// </summary>
	/// <param name="margin">���ý����̽� ���� �׵θ� ����</param>
	/// <param name="colorTextureFile">�׵θ� �ؽ����̹��� ����</param>
	void SetOutline(float margin, wstring colorTextureFile = L"Textures/Color/White.png");
	/// <summary>
	/// ��ư Ŭ����� Ȱ��ȭ
	/// </summary>
	/// <param name="isPressable">true / false �Է�</param>
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