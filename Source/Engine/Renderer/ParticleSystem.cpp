#include "ParticleSystem.h"

namespace meow {

	ParticleSystem g_particleSystem{300};

	void ParticleSystem::Update(float dt)
	{
		for (auto& particle : m_particles)
		{
			if (particle.m_isActive) particle.Update(dt);
		}
	}
	void ParticleSystem::Draw(Renderer& renderer)
	{
		for (auto& particle : m_particles)
		{
			if (particle.m_isActive) particle.Draw(renderer);
		}
	}
	Particle* ParticleSystem::GetFreeParticle()
	{
		for (auto& particle : m_particles)
		{
			if (!particle.m_isActive) return &particle;
		}
		return nullptr;
	}



}