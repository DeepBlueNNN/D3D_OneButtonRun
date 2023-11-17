#pragma once
class  Material
{
public:
	Material();
	Material(wstring shaderFile);
	~Material();

public:
	void Set();
	void GUIRender();

public: // Getter
	string& GetName() { return m_name; }
	MaterialBuffer::Data& GetData() { return m_buffer->Get(); }

public: // Setter
	void SetName(string name) { m_name = name; }
	void SetShader(wstring shaderFile);
	void SetDiffuseMap(wstring textureFile);
	void SetSpecularMap(wstring textureFile);
	void SetNormalMap(wstring textureFile);

public:
	void Save(string file);
	void Load(string file);

private:
	string m_name = "";

	VertexShader* m_vertexShader = nullptr;
	PixelShader*  m_pixelShader  = nullptr;
	Texture*      m_diffuseMap   = nullptr;
	Texture*      m_specularMap  = nullptr;
	Texture*      m_normalMap    = nullptr;

private:
	MaterialBuffer* m_buffer = nullptr;
	string          m_file = "";

};