#pragma once

class  MatrixBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix matrix;
	};
public:
	MatrixBuffer() : ConstBuffer(&m_data, sizeof(Data))
	{
		m_data.matrix = XMMatrixIdentity();
	}
	void Set(Matrix value)
	{
		m_data.matrix = XMMatrixTranspose(value);
	}
private:
	Data m_data;
};

class  ViewBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix view;
		Matrix invView;
	};
public:
	ViewBuffer() : ConstBuffer(&m_data, sizeof(Data))
	{
		m_data.view    = XMMatrixIdentity();
		m_data.invView = XMMatrixIdentity();
	}
	void Set(Matrix view, Matrix invView)
	{
		m_data.view = XMMatrixTranspose(view);
		m_data.invView = XMMatrixTranspose(invView);
	}
private:
	Data m_data;
};

class LightBuffer : public ConstBuffer
{
private:
	struct Light
	{
		Float4 color     = { 1,1,1,1 };
		Float3 direction = { 1,-1,1 };  // ºûÀÇ ¹æÇâ
		float  shininess = 24.0f;        // Á¦°ö°ª
	};
	struct Data
	{
		Light lights[10];

		UINT lightCount = 1;
		Float3 ambientLight = { 0.1f, 0.1f, 0.1f };
		Float3 ambientCeil = { 0.1f, 0.1f, 0.1f };
		float padding = 0.0f;
	};
public:
	LightBuffer() : ConstBuffer(&m_data, sizeof(Data))
	{

	}
	Data& Get() { return m_data; }
	void Init()
	{
		m_data.lights[0].color = { 1.0f,1.0f,1.0f,1.0f };
		m_data.lights[0].direction = { 1.0f,-1.0f,1.0f};
		m_data.lights[0].shininess = { 24.0f};
		m_data.lightCount = 1;
		m_data.ambientLight = { 0.1f, 0.1f, 0.1f };
		m_data.ambientCeil = { 0.1f, 0.1f, 0.1f };
	}

private:
	Data  m_data;
};

class RayBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Float3 pos;
		int    triangleSize;

		Float3 dir;
		float  padding;
	};
public:
	RayBuffer() : ConstBuffer(&m_data, sizeof(Data))
	{

	}
	Data& Get() { return m_data; }

private:
	Data  m_data;
};

class MaterialBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float4 diffuse   = { 1,1,1,1 };
		Float4 specular  = { 1,1,1,1 };
		Float4 ambient   = { 1,1,1,1 };
		Float4 emissive  = { 1,1,1,1 };

		float  shininess = 24.0f;
		int    hasNormalMap = 0;
		float  padding[2] = {};
	};
public:
	MaterialBuffer() : ConstBuffer(&m_data, sizeof(Data))
	{

	}
	Data& Get() { return m_data; }

private:
	Data  m_data;
};

class FloatValueBuffer : public ConstBuffer
{
public:
	FloatValueBuffer() : ConstBuffer(m_values, sizeof(float) * 4)
	{

	}
	float* Get() { return m_values; }

private:
	float  m_values[4] = {};
};

class WeatherBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Float3 velocity = { 1.8f, -30.0f, 0.0f };
		float  distance = 100.0f;

		Float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		Float3 origin = Float3(0.0f, 0.0f, 0.0f);
		float  time = 0.0f;

		Float3 size = { 50.0f, 50.0f, 50.0f };
		float  turbulence = 5.0f;
	};

public:
	WeatherBuffer() : ConstBuffer(&m_data, sizeof(Data))
	{

	}
	Data& Get() { return m_data; }

private:
	Data m_data;
};

class ColorBuffer : public ConstBuffer
{
public:
	ColorBuffer() : ConstBuffer(&m_color, sizeof(Float4))
	{

	}
	Float4& Get() { return m_color; }

private:
	Float4  m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
};
