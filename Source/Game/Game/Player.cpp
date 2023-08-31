#include "Player.h"
#include "Input/InputSystem.h"
#include "Bullet.h"
#include "SpaceGame.h"
#include "Renderer/ModelManager.h"
#include "HealthPad.h"
#include "Audio/AudioSystem.h"
#include "Framework/Framework.h"
#include "Framework/SpriteAnimComponent.h"


namespace meow {

	CLASS_DEFINITION(Player)



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
	void Player::Read(const json_t& value)
 	{
  		Actor::Read(value);
		READ_DATA(value, speed);
 		READ_DATA(value, turnRate);
		INFO_LOG(turnRate << "a")

	}
	void Player::Update(float dt)
	{
		Actor::Update(dt);
		//health update 
		m_scene->GetActorByName("Health")->GetComponent<meow::TextRenderComponent>()->SetText("HEALTH: " + std::to_string((this->getHP())));
		//movement
		float rotate = 0;
		if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
		if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
		//transform.rotation += rotate * turnRate * dt;
 		m_physicsComponent->ApplyTorque(rotate * turnRate);
		float thrust = 0;

		if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) {
			//animation update booster
			if (GetComponent<SpriteAnimComponent>()->sequence != &GetComponent<SpriteAnimComponent>()->sequences["boost"]) {
				GetComponent<SpriteAnimComponent>()->SetSequence("boost");
			}
			thrust = 1;
		}
		//animation update back to idle
		if (GetComponent<SpriteAnimComponent>()->sequence != &GetComponent<SpriteAnimComponent>()->sequences["idle"] && 
			!(meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_W))) 
		{
			GetComponent<SpriteAnimComponent>()->SetSequence("idle");
		}

		meow::Vector2 forward = meow::vec2{ 0,-1 }.Rotate(transform.rotation);


		m_physicsComponent->ApplyForce(forward * speed * thrust);

		// check if position is off screen, if so wrap the position and set physics component to new position
		if ((transform.position.x < 0 || transform.position.x >(float)meow::g_renderer.GetWidth()) ||
			(transform.position.y < 0 || transform.position.y >(float)meow::g_renderer.GetHeight()))
		{
			transform.position.x = meow::Wrap(transform.position.x, (float)meow::g_renderer.GetWidth());
			transform.position.y = meow::Wrap(transform.position.y, (float)meow::g_renderer.GetHeight());

			m_physicsComponent->SetPosition(transform.position);
		}

		int score = m_game->GetScore();
		//fire
		shotcd -= dt;
		if ((meow::g_inputSystem.GetMouseButtonDown(0) && !meow::g_inputSystem.GetMousePreviousButtonDown(0) && shotcd <= 0) || fireburst)
		{

			

			fireburst = true;  //sets to true at beginning and is only set to false once it goes through the bursts
			fireburstdelay -= dt;
			if (fireburstdelay <= 0) { //checking delay between bursts
				fireburstcount--;
				fireburstdelay = 0.10f;
				if (fireburstcount <= 0) { //checking the amount of bursts left if 0 then it wont re enter the burst and it will be on cd
					fireburst = false;
					fireburstcount = fireburstcountmax;
				}

				meow::g_audioSystem.PlayOneShot("RocketBlast");

				auto bullet = INSTANTIATE(Bullet, "PlayerRocket");
				bullet->transform = { transform.position + forward * 30,transform.rotation,1.0f };
				bullet->Initialize();
				bullet->tag = "Player";
				m_scene->Add(std::move(bullet));

				if (score >= 100) {
					CreateBulletScaling(15);
				}
				if (score >= 200) {
					CreateBulletScaling(25);
				}
				if (score >= 300) {
					CreateBulletScaling(35);
				}
				if (score >= 400) {
					CreateBulletScaling(45);
				}
				if (score >= LevelUp)
				{
					fireburstcountmax++;
					LevelUp += 1000;
				}
			}
			shotcd = shotcdcooldown;
		} 
		if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_T)) meow::g_Time.SetTimeScale(0.25f);
		else meow::g_Time.SetTimeScale(1.0f);

		//super
		if (score >= 750) {
			supercd -= dt;															//if burstbool is true then it can bypass the cd and shift check
			if ((meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_LSHIFT) && supercd <= 0) || superburstbool) {


				superburstbool = true;  //sets to true at beginning and is only set to false once it goes through the three bursts 
				superburstdelay -= dt;
				if (superburstdelay <= 0) { //checking delay between bursts
					superburst--;
					superburstdelay = 0.10f;
					if (superburst <= 0) { //checking the amount of bursts left if 0 then it wont re enter the burst and it will be on cd
						superburstbool = false;
						superburst = 3;
					}
					//creates the bullets
					for (int i = 0; i < 20; i++)
					{																			//adding the m_burst for slight rotation offset
						auto super = INSTANTIATE(Bullet, "PlayerRocket");
						super->transform = { transform.position,transform.rotation + meow::DegToRad((float)i * 18 + (superburst * 5)),1.0f };
						super->Initialize();
						m_scene->Add(std::move(super));

					}
					meow::g_audioSystem.PlayOneShot("RocketBlast");
				}
				supercd = superTimer;

			}
		}
	}

	void Player::OnCollisionEnter(Actor* other)
	{
		if (other->tag != tag && other->name == "EnemyLaser") {
			health -= 1;
			//dynamic_cast<SpaceGame*>(m_game)->m_HealthText->Create(meow::g_renderer, "HEALTH: " + std::to_string(this->getHP()), meow::Color{ 1, 1, 1, 1 });
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
		if (health <= 0) {

			//dynamic_cast<SpaceGame*>(m_game)->SetState(SpaceGame::eState::PlayerDeadStart);
			EVENT_DISPATCH("OnPlayerDead", 0);
			destroyed = true;
		}
		if (other->name == "HealthPad" || other->tag == "1") {
			health += dynamic_cast<HealthPad*>(other)->getHealthAmount();
			if (health > maxHealth) {
				health = maxHealth;
			}
			INFO_LOG(health);
			m_scene->GetActorByName("Health")->GetComponent<meow::TextRenderComponent>()->text = "Health: " + std::to_string(this->getHP());
			meow::g_audioSystem.PlayOneShot("HealthPickup");
		}
	}
	void Player::CreateBulletScaling(float offset)
	{
		vec2 shipForward(std::cos(transform.rotation), std::sin(transform.rotation));
		auto bullet = INSTANTIATE(Bullet, "PlayerRocket");
		bullet->transform = { transform.position + shipForward * offset,transform.rotation,1.0f };
		bullet->Initialize();
		m_scene->Add(std::move(bullet));

		auto bullet2 = INSTANTIATE(Bullet, "PlayerRocket");
		bullet2->transform = { transform.position - shipForward * offset,transform.rotation,1.0f };
		bullet2->Initialize();
		m_scene->Add(std::move(bullet2));
	}
}

