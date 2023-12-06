#include "framework.h"
#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride, UINT outputCount)
{
	m_inputData     = inputData;
	m_inputStride   = inputStride;
	m_inputCount    = inputCount;
	m_outputStride  = outputStride;
	m_outputCount   = outputCount;

	CreateInput();
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

StructuredBuffer::~StructuredBuffer()
{
	SAFE_RELEASE(m_input);
	SAFE_RELEASE(m_srv);
	SAFE_RELEASE(m_output);
	SAFE_RELEASE(m_uav);
	SAFE_RELEASE(m_result);
}

void StructuredBuffer::Copy(void* data, UINT size)
{
	DC->CopyResource(m_result, m_output);

	D3D11_MAPPED_SUBRESOURCE  subResource;
	DC->Map(m_result, 0, D3D11_MAP_READ, 0, &subResource);
	{
		memcpy(data, subResource.pData, size);
	}
	DC->Unmap(m_result, 0);
}

void StructuredBuffer::UpdateInput(void* data)
{
	DC->UpdateSubresource(m_input, 0, nullptr, data, m_inputStride, m_inputCount);
	m_srv->Release();
	CreateSRV();
}

void StructuredBuffer::CreateInput()
{
	D3D11_BUFFER_DESC bufferDesc = {};

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = m_inputStride * m_inputCount;

	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = m_inputStride;   // Ãß°¡

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = m_inputData;

	HRESULT hr = DEVICE->CreateBuffer(&bufferDesc, &data, &m_input);
	assert(SUCCEEDED(hr));
}

void StructuredBuffer::CreateSRV()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.Buffer.NumElements = m_inputCount;

	HRESULT hr = DEVICE->CreateShaderResourceView(m_input, &desc, &m_srv);
	assert(SUCCEEDED(hr));
}

void StructuredBuffer::CreateOutput()
{
	D3D11_BUFFER_DESC bufferDesc = {};
	
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = m_outputStride*m_outputCount;
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = m_outputStride;

	HRESULT hr = DEVICE->CreateBuffer(&bufferDesc, nullptr, &m_output);
	assert(SUCCEEDED(hr));
}

void StructuredBuffer::CreateUAV()
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = m_outputCount;
	
	HRESULT hr = DEVICE->CreateUnorderedAccessView(m_output, &uavDesc, &m_uav);
	assert(SUCCEEDED(hr));
}

void StructuredBuffer::CreateResult()
{
	D3D11_BUFFER_DESC bufferDesc = {};
	m_output->GetDesc(&bufferDesc);

	bufferDesc.Usage = D3D11_USAGE_STAGING;
	bufferDesc.BindFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	HRESULT hr = DEVICE->CreateBuffer(&bufferDesc, nullptr, &m_result);
	assert(SUCCEEDED(hr));
}
