#pragma once

class Shader
{
protected:
    Shader() = default;
    virtual ~Shader();

public:
    static class VertexShader*   AddVS(wstring file);
    static class PixelShader*    AddPS(wstring file);
    static class ComputeShader*  AddCS(wstring file);
    static class GeometryShader* AddGS(wstring file);
    static void Delete();

public:
    virtual void Set() = 0;

public:
    wstring  GetFile() { return m_file; }

protected:
    bool     IsExistFile(wstring strFile);

protected:
    ID3DBlob* m_blob = nullptr;
    static unordered_map<wstring, Shader*> m_shaders;
    wstring   m_file = L"";
};