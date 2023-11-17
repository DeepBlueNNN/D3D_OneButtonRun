#include "framework.h"
#include "Spark.h"

Spark::Spark(wstring imageFile, bool isAdditive)
{
	m_material->SetShader(L"Geometry/Spark.hlsl");
	m_material->SetDiffuseMap(imageFile);

	m_geometryShader = Shader::AddGS(L"Geometry/Spark.hlsl");
	m_buffer = new FloatValueBuffer();
	m_startColorBuffer = new ColorBuffer();
	m_endColorBuffer   = new ColorBuffer();

	if (isAdditive)
		m_blendState[1]->Additive();

	Create();
}

Spark::~Spark()
{
	SAFE_DELETE(m_endColorBuffer);
	SAFE_DELETE(m_startColorBuffer);
	SAFE_DELETE(m_buffer);
}

void Spark::Update()
{
	if (!m_isActive) return;

	m_buffer->Get()[0] += DELTA;

	if (m_buffer->Get()[0] > m_buffer->Get()[1])
		Stop();
}

void Spark::Render()
{
	m_buffer->SetVS(10);
	m_buffer->SetPS(10);

	m_startColorBuffer->SetPS(11);
	m_endColorBuffer->SetPS(12);

	__super::Render();
}

void Spark::GUIRender()
{
	ImGui::Text("Spark Option");
	ImGui::SliderInt("ParticleCount", (int*)&m_particleCount, 1, MAX_COUNT);
	ImGui::SliderFloat("Duration", &m_buffer->Get()[1], 0.0f, 10.0f);
	ImGui::SliderFloat("MinRadius", &m_minRadius, 1.0f, m_maxRadius);
	ImGui::SliderFloat("MaxRadius", &m_maxRadius, m_minRadius, 50.0f);
	ImGui::SliderFloat("MinSize", &m_minSize, 0.1f, m_maxSize);
	ImGui::SliderFloat("MaxSize", &m_maxSize, m_minSize, 10.0f);

	ImGui::ColorEdit4("StartColor", (float*)&m_startColorBuffer->Get());
	ImGui::ColorEdit4("EndColor", (float*)&m_endColorBuffer->Get());
}

void Spark::Play(Vector3 position)
{
	m_buffer->Get()[0] = 0.0f;
	__super::Play(position);
	UpdateParticle();
	m_vertexBuffer->Update(m_vertices.data(), m_particleCount);
}

void Spark::Create()
{
	m_vertices.resize(MAX_COUNT);
	m_vertexBuffer = new VertexBuffer(m_vertices.data(), sizeof(VertexParticle), MAX_COUNT);

	m_particleCount = 500;
	m_buffer->Get()[1] = 1.0f;
}


void Spark::UpdateParticle()
{
	for (UINT i = 0; i < m_particleCount; i++)
	{
		float size;
		size = Random(m_minSize, m_maxSize);
		m_vertices[i].position = this->m_position;
		m_vertices[i].size = Float2(size, size);

		float radius = Random(m_minRadius, m_maxRadius);

		Float3 rot;
		rot.x = Random(0.0f, XM_2PI);
		rot.y = Random(0.0f, XM_2PI);
		rot.z = Random(0.0f, XM_2PI);

		Vector3 velocity = Vector3(0.0f, 0.0f, radius);
		Matrix matRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

		XMStoreFloat3(&m_vertices[i].velocity, XMVector3TransformCoord(velocity, matRot));
	}
}
