#include "Framework.h"
#include <io.h>

unordered_map<wstring, Shader*> Shader::m_shaders;

Shader::~Shader()
{
    m_blob->Release();
}

VertexShader* Shader::AddVS(wstring file)
{    
    if (file.size() == 0)
        file = L"Light/Light.hlsl";

    wstring key = file + L"VS";

    if (m_shaders.count(key) > 0)
        return (VertexShader*)m_shaders[key];

    wstring path = L"Shaders/" + file;    

    m_shaders[key] = new VertexShader(path);

    return (VertexShader*)m_shaders[key];
}

PixelShader* Shader::AddPS(wstring file)
{
    if (file.size() == 0)
        file = L"Light/Light.hlsl";
    wstring key = file + L"PS";

    if (m_shaders.count(key) > 0)
        return (PixelShader*)m_shaders[key];

    wstring path = L"Shaders/" + file;

    m_shaders[key] = new PixelShader(path);

    return (PixelShader*)m_shaders[key];
}

ComputeShader* Shader::AddCS(wstring file)
{
    wstring key = file;

    if (m_shaders.count(key) > 0)
        return (ComputeShader*)m_shaders[key];

    wstring path = L"Shaders/" + file;

    m_shaders[key] = new ComputeShader(path);

    return (ComputeShader*)m_shaders[key];

}

GeometryShader* Shader::AddGS(wstring file)
{
    wstring key = file;

    if (m_shaders.count(key) > 0)
        return (GeometryShader*)m_shaders[key];

    wstring path = L"Shaders/" + file;

    m_shaders[key] = new GeometryShader(path);

    return (GeometryShader*)m_shaders[key];
}

void Shader::Delete()
{
    for (pair<wstring, Shader*> shader : m_shaders)
        delete shader.second;
}

bool Shader::IsExistFile(wstring strFile)
{
    string str;
    string message;
    str.assign(strFile.begin(), strFile.end());

    if (_access(str.c_str(), 00) != 0)
    {
        message = "Shader::IsExistFile\n";
        message += "쉐이더파일 없음!\n" + str;
        MessageBoxA(NULL, message.c_str(), "ERROR", MB_OK);
        return false;
    }
    return true;
}
