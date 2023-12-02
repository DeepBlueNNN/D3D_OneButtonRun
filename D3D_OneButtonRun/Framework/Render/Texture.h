#pragma once
class Texture
{
public:
	Texture(ID3D11ShaderResourceView* srv, ScratchImage& image, wstring file);
	~Texture();

public: // Setter
	void PSSet(UINT slot = 0);

public: // Getter
	Vector2 GetSize()
	{
		return Vector2(m_image.GetMetadata().width, m_image.GetMetadata().height);
	}
	wstring GetFile() {return m_file;}
	ID3D11ShaderResourceView* GetSRV() { return m_srv; }

public:
	static Texture* Add(wstring file);
	static Texture* Add(wstring file, wstring key);
	static Texture* Add(wstring key, ID3D11ShaderResourceView* srv);
	static void Delete();
	void   ReadPixels(vector<Float4>& pixels);  // terrain(heigh map)
	static bool	IsExistFile(wstring strFile);

private:
	wstring	m_file = L"";
	ScratchImage	m_image;
	ID3D11ShaderResourceView* m_srv = nullptr;

	static unordered_map<wstring, Texture*>	m_textures;
};