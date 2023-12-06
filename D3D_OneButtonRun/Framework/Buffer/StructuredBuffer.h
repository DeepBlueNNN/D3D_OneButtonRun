#pragma once
class  StructuredBuffer
{
public:
	StructuredBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT ouputStride, UINT ouputCount);
	~StructuredBuffer();

public:
	void Copy(void* data, UINT size);
	void UpdateInput(void* data);

	ID3D11UnorderedAccessView*& GetUAV() { return m_uav; }
	ID3D11ShaderResourceView*&  GetSRV() { return m_srv; }

private:
	void CreateInput();
	void CreateSRV();
	void CreateOutput();
	void CreateUAV();
	void CreateResult();

private:
	ID3D11Buffer*				m_input = nullptr;    // GPU->input
	ID3D11ShaderResourceView*	m_srv   = nullptr;

	ID3D11Buffer*				m_output = nullptr;   // GPU->output
	ID3D11UnorderedAccessView*	m_uav = nullptr;
	
	ID3D11Buffer*				m_result = nullptr;;  // ouput->result

	void* m_inputData  = nullptr;
	UINT  m_inputStride = 0;
	UINT  m_inputCount  = 0;
	UINT  m_outputStride = 0;
	UINT  m_outputCount = 0;
};