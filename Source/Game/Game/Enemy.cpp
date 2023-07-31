#include "Enemy.h"
#include "Renderer/Renderer.h"
#include "Player.h"
#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Bullet.h"
#include "Framework/Emitter.h"
#include "Renderer/ModelManager.h"

void Enemy::Update(float dt)
{

	Actor::Update(dt);
	meow::Vector2 forward = meow::vec2{ 0,-1 }.Rotate(m_transform.rotation);
	Player* player = m_scene->GetActor<Player>();
	if (player) {
		meow::Vector2 direction = player->m_transform.position - m_transform.position;

		float turnAngle = meow::vec2::SignedAngle(forward, direction.Normalized());

		m_transform.rotation += turnAngle * dt;

		if (std::fabs(turnAngle) < meow::DegToRad(30.0f)) 
		{
			if (m_fireTimer <= 0) {
				meow::Transform transform{ m_transform.position,m_transform.rotation,1 };

				std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(400.0f, transform, meow::g_ModelManager.Get("bullet.txt"));
				bullet->m_tag = "Enemy";
				m_scene->Add(std::move(bullet));
				m_fireTimer = m_fireRate;
			}
		}
		m_fireTimer -= dt;

	}

	
	m_transform.position += forward * m_speed * 1 * dt;
	m_transform.position.x = meow::Wrap(m_transform.position.x, (float)meow::g_renderer.GetWidth());
	m_transform.position.y = meow::Wrap(m_transform.position.y, (float)meow::g_renderer.GetHeight());
	
}

void Enemy::onCollision(Actor* other)
{
	if (other->m_tag != m_tag && other->m_id == "bullet") {
		m_destroyed = true;
		m_game->AddPoints(1);

		meow::EmitterData pInfo;
		pInfo.burst = true;
		pInfo.burstCount = 30;
		pInfo.spawnRate = 300;
		pInfo.angle = 0;
		pInfo.angleRange = meow::Pi;
		pInfo.lifetimeMin = 0.5f;
		pInfo.lifetimeMax = 1.0f;
		pInfo.speedMin = 50;
		pInfo.speedMax = 100;
		pInfo.damping = 0.25f;
		pInfo.color = meow::Color{ 0,1,1,1 };
		auto emitter = std::make_unique<meow::Emitter>(m_transform, pInfo);
		emitter->m_lifespan = 0.5f;
		m_scene->Add(std::move(emitter));
	}
}
