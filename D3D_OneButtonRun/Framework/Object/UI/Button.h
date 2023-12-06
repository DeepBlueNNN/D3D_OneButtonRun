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
	/// Hover�� ����� �̹��� �߰�
	/// </summary>
	/// <param name="hoverTextureFile">�̹��� ���ϰ��</param>
	void SetHoverImage(wstring hoverTextureFile) { m_hoverImage = hoverTextureFile; }
	/// <summary>
	/// Ŭ�� �� �̵� Scene �̸� ����
	/// </summary>
	/// <param name="sceneName"></param>
	void SetNextScene(string sceneName) { m_nextSceneName = sceneName; }
	/// <summary>
	/// ��ư �̹��� �߰�
	/// </summary>
	/// <param name="subTextureFile">�̹��� ���ϰ��</param>
	void SetSubImage(wstring subTextureFile);

private:
	ButtonAction m_actionType = MAX;
	Texture* m_subImage = nullptr;
	wstring m_buttonImage = L"";
	wstring m_hoverImage = L"";
	string m_nextSceneName = "";
};