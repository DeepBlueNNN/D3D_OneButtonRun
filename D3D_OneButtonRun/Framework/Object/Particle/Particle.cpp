#include "framework.h"
#include "Particle.h"

Particle::Particle()
{
	m_material = new Material();

	m_blendState[0] = new BlendState();
	m_blendState[1] = new BlendState();

	// alpha°ª 0.5
//	m_blendState[1]->AlphaToCoverage(true);
	m_blendState[1]->Alpha(true);

	m_depthState[0] = new DepthStencilState();
	m_depthState[1] = new DepthStencilState();

	m_depthState[1]->DepthWriteMask();
}

Particle::~Particle()
{
	SAFE_DELETE(m_material);
	SAFE_DELETE(m_vertexBuffer);

	SAFE_DELETE(m_blendState[0]);
	SAFE_DELETE(m_blendState[1]);

	SAFE_DELETE(m_depthState[0]);
	SAFE_DELETE(m_depthState[1]);
}

void Particle::Render()
{
	if (!m_isActive)
		return;

	m_blendState[1]->SetState();
	m_depthState[1]->SetState();
	{
		m_vertexBuffer->Set(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		m_material->Set();
		m_geometryShader->Set();
		DC->Draw(1000, 0);
		DC->GSSetShader(nullptr, nullptr, 0);
	}
	m_blendState[0]->SetState();
	m_depthState[1]->SetState();
}

void Particle::Play(Vector3 position)
{
	m_isActive = true;
	this->m_position = position;
}

void Particle::Stop()
{
	m_isActive = false;
}
