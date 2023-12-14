#pragma once
class SubMenu : public Quad
{
protected:
	typedef VertexUV VertexType;

public:
	SubMenu();
	~SubMenu();

public:
	void Update();
	void Render();
	void GUIRender();

public:	// Getter
	bool IsSubMenuOn() { return m_isSubMenuOn; }

public:	// Setter
	void SetSubMenuOn(bool set) { m_isSubMenuOn = set; }

private:
	void MakeMesh();

private:
	Texture* m_backGround = nullptr;
	FloatValueBuffer* m_valueBuffer = nullptr;
	vector<Button*> m_buttons;
	bool m_isSubMenuOn = false;
};