#include    "framework.h"
#include    "IndexBuffer.h"

IndexBuffer::IndexBuffer(void* data, UINT count)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(UINT) * count;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = data;

	DEVICE->CreateBuffer(&bufferDesc, &initData, &m_pBuffer);
}

IndexBuffer::~IndexBuffer()
{
	SAFE_RELEASE(m_pBuffer);
}

void IndexBuffer::Update(void * data, UINT count)
{
	DC->UpdateSubresource(m_pBuffer, 0, nullptr, data, sizeof(UINT), count);
}

void IndexBuffer::Set()
{
	DC->IASetIndexBuffer(m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
}
