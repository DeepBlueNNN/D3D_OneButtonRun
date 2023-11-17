#include    "framework.h"
#include    "VertexBuffer.h"

VertexBuffer::VertexBuffer(void* data, UINT stride, UINT count)
{
	m_stride = stride;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;    // DC & DEVICE  read/write
	bufferDesc.ByteWidth = stride * count;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = data;

	DEVICE->CreateBuffer(&bufferDesc, &initData, &m_buffer);
}

VertexBuffer::~VertexBuffer()
{
	SAFE_RELEASE(m_buffer);
}

void VertexBuffer::Update(void* data, UINT count)
{
	DC->UpdateSubresource(m_buffer, 0, nullptr, data, m_stride, count);
}

void VertexBuffer::Set(D3D11_PRIMITIVE_TOPOLOGY type)
{
	DC->IASetVertexBuffers(0, 1, &m_buffer, &m_stride, &m_offset);
	DC->IASetPrimitiveTopology(type);
}

void VertexBuffer::Set(UINT slot, D3D11_PRIMITIVE_TOPOLOGY type)
{
	DC->IASetVertexBuffers(slot, 1, &m_buffer, &m_stride, &m_offset);
	DC->IASetPrimitiveTopology(type);
}

