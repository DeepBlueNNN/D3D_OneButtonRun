#include "framework.h"
#include "ComputeShader.h"

ComputeShader::ComputeShader(wstring file)
{
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    D3DCompileFromFile(file.c_str(),
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CS", "cs_5_0", flags, 0, &m_blob, nullptr);

    DEVICE->CreateComputeShader(m_blob->GetBufferPointer(),
        m_blob->GetBufferSize(), nullptr, &m_shader);
}

ComputeShader::~ComputeShader()
{
    SAFE_RELEASE(m_shader);
}

void ComputeShader::Set()
{
    DC->CSSetShader(m_shader, nullptr, 0);
}
