#include "framework.h"
#include "Texture.h"
#include "Framework/Utilities/StringPath.h"
#include <io.h>

unordered_map<wstring, Texture*> Texture::m_textures;

Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image, wstring file)
{
	m_srv = srv;
	m_image = move(image);
	m_file = file;
}

Texture::~Texture()
{
	SAFE_RELEASE(m_srv);
}

void Texture::PSSet(UINT slot)
{
	DC->PSSetShaderResources(slot, 1, &m_srv);
}

Texture* Texture::Add(wstring file)
{
	if (m_textures.count(file) > 0)
		return m_textures[file];

	if (!IsExistFile(file))
		assert(false);

	ScratchImage image;
	HRESULT result;

	wstring extension = StringPath::GetExtension(file);

	if(extension.compare(L"tga") == 0)       // fbx
		result = LoadFromTGAFile(file.c_str(), nullptr, image);
	else if (extension.compare(L"dds") == 0)  // cubemap
		result = LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
	else
	    result = LoadFromWICFile(file.c_str(), WIC_FLAGS_NONE, nullptr, image); // png,jpg,bmp,gif
	assert(SUCCEEDED(result));

	ID3D11ShaderResourceView* srv;

	CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), &srv);

	m_textures[file] = new Texture(srv, image, file);

	return m_textures[file];
}

Texture* Texture::Add(wstring file, wstring key)
{
	if (m_textures.count(key) > 0)
		return m_textures[key];

	if (!IsExistFile(file))
		assert(false);

	ScratchImage image;
	HRESULT result;

	wstring extension = StringPath::GetExtension(file);

	if (extension.compare(L"tga") == 0)       // fbx
		result = LoadFromTGAFile(file.c_str(), nullptr, image);
	else if (extension.compare(L"dds") == 0)  // cubemap
		result = LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
	else
		result = LoadFromWICFile(file.c_str(), WIC_FLAGS_NONE, nullptr, image); // png,jpg,bmp,gif
	assert(SUCCEEDED(result));

	ID3D11ShaderResourceView* srv;

	CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), &srv);

	m_textures[key] = new Texture(srv, image, file);

	return m_textures[key];
}


Texture* Texture::Add(wstring key, ID3D11ShaderResourceView* srv)
{
	if (m_textures.count(key) > 0)
		return m_textures[key];

	ScratchImage image;

	m_textures[key] = new Texture(srv, image, key);

	return m_textures[key];
}

void Texture::Delete()
{
	for (pair<wstring, Texture*> texture : m_textures)
		SAFE_DELETE(texture.second);
}

void Texture::ReadPixels(vector<Float4>& pixels)
{
	uint8_t* colors = m_image.GetPixels();   // 0~255
	UINT size = m_image.GetPixelsSize();

	pixels.resize(size / 4);  // rgba
	float scale = 1.0f / 255.0f;

	for (UINT i = 0; i < pixels.size(); i++)
	{
		pixels[i].x = colors[i * 4 + 0] * scale;
		pixels[i].y = colors[i * 4 + 1] * scale;
		pixels[i].z = colors[i * 4 + 2] * scale;
		pixels[i].w = colors[i * 4 + 3] * scale;
	}
}

bool Texture::IsExistFile(wstring strFile)
{
	string str;
	string message;
	str.assign(strFile.begin(), strFile.end());

	if (_access(str.c_str(), 00) != 0)
	{
		message = "Texture::IsExistFile\n";
		message += "텍스쳐파일 없음\n" + str;
		MessageBoxA(NULL, message.c_str(), "ERROR", MB_OK);
		return false;
	}
	return true;
}
