#pragma once
class ConstBuffer
{
protected:
	ConstBuffer(void* data, UINT dataSize);
	virtual ~ConstBuffer();

public:
	void  SetVS(UINT slot);
	void  SetPS(UINT slot);
	void  SetCS(UINT slot);

private:
	ID3D11Buffer*	m_buffer = nullptr;
	void*			m_data = nullptr;
	UINT			m_dataSize = 0;

	D3D11_MAPPED_SUBRESOURCE m_subResource;
};