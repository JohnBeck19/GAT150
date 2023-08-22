#include "Player.h"
#include "Input/InputSystem.h"
#include "Bullet.h"
#include "SpaceGame.h"
#include "Renderer/ModelManager.h"
#include "HealthPad.h"
#include "Audio/AudioSystem.h"
#include "Framework/Framework.h"


bool Player::Initialize()
{
	Actor::Initialize();
	m_physicsComponent = GetComponent<meow::PhysicsComponent>();
	auto CollisionComponent = GetComponent<meow::CollisionComponent>();
	if (CollisionComponent) {
		auto renderComponent = GetComponent<meow::RenderComponent>();
		if (renderComponent) {

			float scale = transform.scale;
			CollisionComponent->m_radius = renderComponent->GetRadius();
		}
	}
	
	return true;
}

void Player::Update(float dt)
{
	Actor::Update(dt);
	//movement
	float rotate = 0;
	if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	transform.rotation += rotate * m_turnRate * dt;

	float thrust = 0;
	if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

	meow::Vector2 forward = meow::vec2{ 0,-1 }.Rotate(transform.rotation);
	

	m_physicsComponent->ApplyForce(forward * m_speed * thrust);

	//m_transform.position += forward * m_speed * thrust * dt;
	transform.position.x = meow::Wrap(transform.position.x, (float)meow::g_renderer.GetWidth());
	transform.position.y = meow::Wrap(transform.position.y, (float)meow::g_renderer.GetHeight());

	//fire

	if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !meow::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
	{
		meow::Transform transform1{ transform.position,transform.rotation + meow::DegToRad(10),1 };
		std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(400.0f, transform1);
		bullet->tag = "Player";
		std::unique_ptr<meow::SpriteComponent> component = std::make_unique<meow::SpriteComponent>();
		component->m_texture = GET_RESOURCE(meow::Texture, "Rocket.png", meow::g_renderer);
		bullet->AddComponent(std::move(component));

		auto CollisionComponent = std::make_unique<meow::CircleCollisionComponent>();
		bullet->AddComponent(std::move(CollisionComponent));

		bullet->Initialize();
		m_scene->Add(std::move(bullet));


		meow::Transform transform2{ transform.position,transform.rotation + meow::DegToRad(-10),1 };
		bullet = std::make_unique<Bullet>(400.0f, transform2);
		component = std::make_unique<meow::SpriteComponent>();
		component->m_texture = GET_RESOURCE(meow::Texture, "Rocket.png", meow::g_renderer);
		bullet->AddComponent(std::move(component));

		CollisionComponent = std::make_unique<meow::CircleCollisionComponent>();
		bullet->AddComponent(std::move(CollisionComponent));

		bullet->Initialize();
		bullet->tag = "Player";
		m_scene->Add(std::move(bullet));
	}
	if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_T)) meow::g_Time.SetTimeScale(0.25f);
	else meow::g_Time.SetTimeScale(1.0f);

	//super
	m_supercd -= dt;															//if burstbool is true then it can bypass the cd and shift check
	if ((meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_LSHIFT) && m_supercd <= 0) || m_burstbool ) {
		std::unique_ptr<Bullet> super;

		m_burstbool = true;  //sets to true at beginning and is only set to false once it goes through the three bursts 
		m_burstdelay -= dt;
		if (m_burstdelay <= 0) { //checking delay between bursts
			m_burst--;
			m_burstdelay = 0.10f;
			if (m_burst <= 0) { //checking the amount of bursts left if 0 then it wont re enter the burst and it will be on cd
				m_burstbool = false;
				m_burst = 3;
			}
			//creates the bullets
			for (int i = 0; i < 20; i++)
			{																			//adding the m_burst for slight rotation offset
				meow::Transform transform2{ transform.position,transform.rotation + meow::DegToRad((float)i * 18+(m_burst*5)),.5f };
				super = std::make_unique<Bullet>(400.0f, transform2);
				super->tag = "Player";
				std::unique_ptr<meow::SpriteComponent> component = std::make_unique<meow::SpriteComponent>();
				component->m_texture = GET_RESOURCE(meow::Texture, "Rocket.png", meow::g_renderer);
				super->AddComponent(std::move(component));

				auto CollisionComponent = std::make_unique<meow::CircleCollisionComponent>();
				super->AddComponent(std::move(CollisionComponent));

				super->Initialize();
				m_scene->Add(std::move(super));
			}
			meow::g_audioSystem.PlayOneShot("Laser");
		}
		m_supercd = m_superTimer;
		
		}
	
}

void Player::onCollision(Actor* other)
{
	if (other->tag != tag && other->id == "bullet") {
		m_health -= 1;
		dynamic_cast<SpaceGame*>(m_game)->m_HealthText->Create(meow::g_renderer, "HEALTH: " + std::to_string(this->getHP()), meow::Color{ 1, 1, 1, 1 });
		meow::g_audioSystem.PlayOneShot("Explosion");
		meow::EmitterData pInfo;
		pInfo.burst = true;
		pInfo.burstCount = 50;
		pInfo.spawnRate = 300;
		pInfo.angle = 0;
		pInfo.angleRange = meow::Pi;
		pInfo.lifetimeMin = 0.5f;
		pInfo.lifetimeMax = 1.0f;
		pInfo.speedMin = 50;
		pInfo.speedMax = 150;
		pInfo.damping = 0.25f;
		pInfo.color = meow::Color{ 255,0,0,1 };
		auto emitter = std::make_unique<meow::Emitter>(transform, pInfo);
		emitter->lifespan = 0.5f;
		m_scene->Add(std::move(emitter));
	}
	if (m_health <= 0) {
		
		dynamic_cast<SpaceGame*>(m_game)->SetState(SpaceGame::eState::PlayerDeadStart);
		m_destroyed = true;
	}
	if (other->tag == "HealthPad") {
		m_health = dynamic_cast<HealthPad*>(other)->getHealthAmount();
		if (m_health < m_maxHealth) {
			m_health = m_maxHealth;
		}
		dynamic_cast<SpaceGame*>(m_game)->m_HealthText->Create(meow::g_renderer, "HEALTH: " + std::to_string(this->getHP()), meow::Color{ 1, 1, 1, 1 });
	}
}
