#include "Enemy.h"
#include "Renderer/Renderer.h"
#include "Player.h"
#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Bullet.h"
#include "Framework/Emitter.h"
#include "Renderer/ModelManager.h"
#include "Framework/Framework.h"
namespace meow {

	CLASS_DEFINITION(Enemy)



	bool Enemy::Initialize()
	{
		Actor::Initialize();
		m_physicsComponent = GetComponent<PhysicsComponent>(); 
		auto collisionComponent = GetComponent<CollisionComponent>();
		if (collisionComponent)
		{
			auto renderComponent = GetComponent<meow::RenderComponent>();
			if (renderComponent)
			{
				float scale = transform.scale;
				collisionComponent->m_radius = renderComponent->GetRadius() * scale;
			}
		}
		
		return true;
	}
	void Enemy::OnDestroy()
	{
		Actor::OnDestroy();
	}
	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);
		READ_DATA(value, speed);
		READ_DATA(value, turnRate);
		READ_DATA(value, fireRate);
	}

	void Enemy::Update(float dt)
	{

		Actor::Update(dt);
		meow::Vector2 forward = meow::vec2{ 0,-1 }.Rotate(transform.rotation);
		Player* player = m_scene->GetActor<Player>();
		if (player) {
			meow::Vector2 direction = player->transform.position - transform.position;

			float turnAngle = meow::vec2::SignedAngle(forward, direction.Normalized());

			//transform.rotation += turnAngle * dt;
			m_physicsComponent->ApplyTorque(turnAngle * turnRate);

			m_physicsComponent->ApplyForce(forward * speed);

			//transform.position += forward * speed * 1 * dt;
			transform.position.x = meow::Wrap(transform.position.x, (float)meow::g_renderer.GetWidth());
			transform.position.y = meow::Wrap(transform.position.y, (float)meow::g_renderer.GetHeight());


			if (std::fabs(turnAngle) < meow::DegToRad(30.0f))
			{
     				if (fireTimer <= 0) {

					auto bullet = INSTANTIATE(Bullet, "EnemyBullet");
					bullet->transform = { transform.position,transform.rotation,3.0f };
					bullet->Initialize();
					m_scene->Add(std::move(bullet));

					if (enemyUpgrade)
					{
						vec2 shipForward(std::cos(transform.rotation), std::sin(transform.rotation));
						
						auto bullet = INSTANTIATE(Bullet, "EnemyBullet");
						bullet->transform = { transform.position + shipForward * 30,transform.rotation,3.0f };
						bullet->Initialize();
						m_scene->Add(std::move(bullet));
						auto bullet2 = INSTANTIATE(Bullet, "EnemyBullet");
						bullet2->transform = { transform.position + shipForward * 30,transform.rotation,3.0f };
						bullet2->Initialize();
						m_scene->Add(std::move(bullet2));

					}
					fireTimer = fireRate;
				}
			}
			fireTimer -= dt;

		}

	}

	void Enemy::OnCollisionEnter(Actor* other)
	{
		if (other->tag != tag && other->name == "Bullet")
		{
			destroyed = true;
			m_game->AddPoints(100);
			m_scene->GetActorByName("Score")->GetComponent<meow::TextRenderComponent>()->SetText("SCORE: " + std::to_string((m_game->GetScore())));
			


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
			auto emitter = std::make_unique<meow::Emitter>(transform, pInfo);
			emitter->lifespan = 0.5f;
			m_scene->Add(std::move(emitter));
		}
	}

}

