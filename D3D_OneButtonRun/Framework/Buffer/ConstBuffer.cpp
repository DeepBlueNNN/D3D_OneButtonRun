#include    "framework.h"
#include    "ConstBuffer.h"

ConstBuffer::ConstBuffer(void* data, UINT dataSize)
{
	m_data = data;
	m_dataSize = dataSize;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = dataSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	DEVICE->CreateBuffer(&bufferDesc, nullptr, &m_buffer);
}

ConstBuffer::~ConstBuffer()
{
	SAFE_RELEASE(m_buffer);
}

void ConstBuffer::SetVS(UINT slot)
{
    DC->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0,&m_subResource);
    memcpy(m_subResource.pData, m_data, m_dataSize);
	DC->Unmap(m_buffer, 0);

	DC->VSSetConstantBuffers(slot, 1, &m_buffer);
}

void ConstBuffer::SetPS(UINT slot)
{
	DC->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_subResource);
	memcpy(m_subResource.pData, m_data, m_dataSize);
	DC->Unmap(m_buffer, 0);

	DC->PSSetConstantBuffers(slot, 1, &m_buffer);
}

void ConstBuffer::SetCS(UINT slot)
{
	DC->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_subResource);
	memcpy(m_subResource.pData, m_data, m_dataSize);
	DC->Unmap(m_buffer, 0);

	DC->CSSetConstantBuffers(slot,1, &m_buffer);
}
