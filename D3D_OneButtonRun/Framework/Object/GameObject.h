#pragma once
class GameObject : public Transform
{
public:
	GameObject(wstring shaderFile = L"Basic/Texture.hlsl");
	~GameObject();

public:
	virtual void Render() {};
	virtual void GUIRender();

public: // Getter
	Material* GetMaterial() { return m_material; };

public: // Setter
	virtual void SetRender();

protected:
	Material*		m_material = nullptr;
	MatrixBuffer*	m_worldBuffer = nullptr;
};
