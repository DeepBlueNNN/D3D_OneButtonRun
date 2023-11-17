#include "framework.h"
#include "SpriteParticle.h"

SpriteParticle::SpriteParticle(wstring imageFile, float width, float height, UINT frameCol, UINT frameRow, bool isAdditive)
{
	m_size.x = width;
	m_size.y = height;

	m_material->SetShader(L"Geometry/SpriteParticle.hlsl");
	m_material->SetDiffuseMap(imageFile);

	// IA -> VS -> (GS) -> RS -> PS -> OM
	m_geometryShader = Shader::AddGS(L"Geometry/SpriteParticle.hlsl");

	m_buffer = new SpriteBuffer();
	m_buffer->Get().maxFrame = { (float)frameCol, (float)frameRow };
	m_maxFrameCount = frameCol * frameRow;

	if (isAdditive)
		m_blendState[1]->Additive();

	Create();
}

SpriteParticle::~SpriteParticle()
{
	SAFE_DELETE(m_buffer);
}

void SpriteParticle::Update()
{
	if (!m_isActive)
		return;

	m_time += (m_speed * DELTA);

	if (m_time > 0.1f)
	{
		m_curFrameCount++;
		m_buffer->Get().curFrame.x = (m_curFrameCount % (UINT)m_buffer->Get().maxFrame.x);
		m_buffer->Get().curFrame.y = (m_curFrameCount / (UINT)m_buffer->Get().maxFrame.y);
		m_time = 0.0f;
	}

	if (m_curFrameCount >= m_maxFrameCount)
		Stop();
}

void SpriteParticle::Render()
{
	m_buffer->SetPS(10);
	__super::Render();
}

void SpriteParticle::GUIRender()
{
}

void SpriteParticle::Play(Vector3 position)
{
	m_isActive = true;

	m_time = 0.0f;
	m_speed = 1.0f;
	m_curFrameCount = 0;

	m_vertex.pos = position;
	m_vertex.pos.y += m_size.y * 0.5f;	// Particle 위치조정
	m_vertex.uv = Float2(m_size.x, m_size.y);

	m_vertexBuffer->Update(&m_vertex, m_particleCount);
}

void SpriteParticle::Create()
{
	m_particleCount = 1;
	m_vertexBuffer = new VertexBuffer(&m_vertex, sizeof(VertexUV), 1);
}
