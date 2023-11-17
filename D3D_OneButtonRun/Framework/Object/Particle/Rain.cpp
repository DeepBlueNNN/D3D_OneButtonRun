#include "framework.h"
#include "Rain.h"

Rain::Rain()
{
	m_material->SetShader(L"Geometry/Rain.hlsl");
	m_material->SetDiffuseMap(L"Textures/Effect/Rain.png");

	m_geometryShader = Shader::AddGS(L"Geometry/Rain.hlsl");
	m_buffer = new WeatherBuffer();

	Create();
}

Rain::~Rain()
{
	SAFE_DELETE(m_buffer);
}

void Rain::Update()
{
	m_buffer->Get().time += DELTA;
}

void Rain::Render()
{
	m_buffer->SetVS(10);
	__super::Render();
}

void Rain::GUIRender()
{
	ImGui::Text("Rain Option");
	ImGui::DragFloat3("Velocity", (float*)&m_buffer->Get().velocity, 0.1f);
	ImGui::SliderFloat("Distance", &m_buffer->Get().distance, 0.0f, 500.0f);
	ImGui::DragFloat3("Origin", (float*)&m_buffer->Get().origin, 0.1f);
	ImGui::DragFloat3("Size", (float*)&m_buffer->Get().size, 0.1f);

	ImGui::ColorEdit4("Color", (float*)&m_buffer->Get().color);
}

void Rain::Create()
{
	m_particleCount = MAX_COUNT;
	m_vertices.resize(m_particleCount);

	for (UINT i = 0; i < m_particleCount; i++)
	{
		Float2 size;
		size.x = Random(0.1f, 0.4f);
		size.y = Random(4.0f, 8.0f);

		Float3 pos;
		pos.x = Random(((m_buffer->Get().size.x) * -1.0f), (m_buffer->Get().size.x));
		pos.y = Random(((m_buffer->Get().size.y) * -1.0f), (m_buffer->Get().size.y));
		pos.z = Random(((m_buffer->Get().size.z) * -1.0f), (m_buffer->Get().size.z));

		m_vertices[i].pos = pos;
		m_vertices[i].uv = size;
	}

	m_vertexBuffer = new VertexBuffer(m_vertices.data(), sizeof(VertexUV), m_particleCount);
}
