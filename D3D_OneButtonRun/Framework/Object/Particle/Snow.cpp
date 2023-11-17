#include "framework.h"
#include "Snow.h"

Snow::Snow()
{
	m_material->SetShader(L"Geometry/Snow.hlsl");
	m_material->SetDiffuseMap(L"Textures/Effect/Snow.png");

	m_geometryShader = Shader::AddGS(L"Geometry/Snow.hlsl");
	m_buffer = new WeatherBuffer();

	Create();
}

Snow::~Snow()
{
	SAFE_DELETE(m_buffer);
}

void Snow::Update()
{
	m_buffer->Get().time += DELTA;
}

void Snow::Render()
{
	m_buffer->SetVS(10);
	__super::Render();
}

void Snow::GUIRender()
{
	ImGui::Text("Snow Option");
	ImGui::DragFloat3("Velocity", (float*)&m_buffer->Get().velocity, 0.1f);
	ImGui::SliderFloat("Distance", &m_buffer->Get().distance, 0.0f, 500.0f);
	ImGui::DragFloat3("Origin", (float*)&m_buffer->Get().origin, 0.1f);
	ImGui::DragFloat3("Size", (float*)&m_buffer->Get().size, 0.1f);
	ImGui::SliderFloat("Turbulence", &m_buffer->Get().distance, 0.0f, 500.0f);

	ImGui::ColorEdit4("Color", (float*)&m_buffer->Get().color);
}

void Snow::Create()
{
	m_particleCount = MAX_COUNT;
	m_vertices.resize(m_particleCount);

	for (UINT i = 0; i < m_particleCount; i++)
	{
		Float2 size;
		size.x = Random(0.5f, 2.0f);
		size.y = size.x;

		Float3 pos;
		pos.x = Random(((m_buffer->Get().size.x) * -1.0f), (m_buffer->Get().size.x));
		pos.y = Random(((m_buffer->Get().size.y) * -1.0f), (m_buffer->Get().size.y));
		pos.z = Random(((m_buffer->Get().size.z) * -1.0f), (m_buffer->Get().size.z));

		m_vertices[i].pos = pos;
		m_vertices[i].size = size;

		m_vertices[i].random.x = Random(0.1f, 1.0f);
		m_vertices[i].random.y = Random(0.1f, 1.0f);
	}

	m_vertexBuffer = new VertexBuffer(m_vertices.data(), sizeof(VertexSnow), m_particleCount);
}
