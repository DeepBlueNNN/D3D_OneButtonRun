#pragma once
//////////////////////////////////////////
// �߻�ȭ Class ����
// C++ CalssƯ¡
//   1. ĸ��ȭ : ������,����� ���� ��ҿ� �� �� �ִ�
//   2. ������ : override, overloading
//   3. ����ȭ : public,private, protected�� ����ó,����� ����ȭ
//   4. ��Ӽ�/�߻�ȭ
//  <�߻�ȭ>
//      1. virtual �Լ��� �ϳ��� ������ �߻�ȭ class��� �Ѵ�
//      2. ��ü�� ������ �� ���� ��
//      3. �� �߻�ȭ Class�� ����ϴ°�?
//          --> ǥ��,��������� ���� ��� �Ҽ� �ְ�
///////////////////////////////////////////////

class Scene
{
public:
	virtual void Update()		= 0; // ���� ���� �Լ�
	virtual void Render()		= 0; // ��ӹ޴� Ŭ�������� �ݵ�� Implement�ؾ���
	virtual void ChangeScene()	{};
	virtual void PreRender()	{};
	virtual void PostRender()	{};
	virtual void GUIRender()	{};

public: // Setter(inline)
	void SetName(string name)		{ m_sceneName = name; }
	void SetNextName(string name)	{ m_nextSceneName = name; }
	void SetActive(bool active)		{ m_active = active; }
	void SetReset(bool value)		{ m_reset = value; }

public: // Getter
	string	GetName()				{ return m_sceneName; }
	auto	GetNextSceneName()		{ return m_nextSceneName; }
	bool	IsActive()				{ return m_active; }
	bool	IsDisplay()				{ return m_display; }
	int		GetSceneID()			{ return m_sceneID; }

protected:
	string	m_sceneName = "";		// ���� Scene �̸�
	string	m_nextSceneName = "";	// ���� Scene �̸�
	bool	m_active = true;		// ���� Scene�� Ȱ��ȭ ����
	bool	m_reset = true;			// Goal �������� ��� ������ Scene�� ��� �� ChangeScene�� ȣ�� �� �� Reset
	bool	m_display = false;		// Map�� �����͸� ǥ�� ����
	int		m_sceneID = 0;
};

