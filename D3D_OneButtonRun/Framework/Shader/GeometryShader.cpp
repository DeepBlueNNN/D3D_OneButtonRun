#include "framework.h"
#include "GeometryShader.h"

GeometryShader::GeometryShader(wstring file)
{
    if (!IsExistFile(file))
        assert(false);

    m_file = file;

    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* outError = nullptr;
    HRESULT hr;

    hr = D3DCompileFromFile(file.c_str(),
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "GS", "gs_5_0", flags, 0, &m_blob, &outError);

    if (outError)
    {
        string str = (char*)outError->GetBufferPointer();
        MessageBoxA(NULL, str.c_str(), "GS : 쉐이더 파일 에러", MB_OK);
    }
    assert(SUCCEEDED(hr));

    hr = DEVICE->CreateGeometryShader(m_blob->GetBufferPointer(),
        m_blob->GetBufferSize(), nullptr, &m_shader);
}

GeometryShader::~GeometryShader()
{
    SAFE_RELEASE(m_shader);
}

void GeometryShader::Set()
{
    DC->GSSetShader(m_shader, nullptr, 0);
}
