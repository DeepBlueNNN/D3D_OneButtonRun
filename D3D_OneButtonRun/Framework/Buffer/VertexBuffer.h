#pragma once
class VertexBuffer
{
public:
	VertexBuffer(void* data, UINT Stride, UINT count);
	virtual ~VertexBuffer();

public:
	void Update(void* data, UINT count);  // Buffer내용 변경시 수정
	void Set(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void Set(UINT solt,D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

private:
	ID3D11Buffer*  m_buffer = nullptr;
	UINT           m_stride = 0;     // pipe line   stride = sizeof(Vector)
	UINT           m_offset = 0;
};