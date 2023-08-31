#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Renderer/Text.h"
#include "Renderer/ModelManager.h"
#include "HealthPad.h"
#include "Framework/Framework.h"



bool SpaceGame::Initialize()
{

	meow::g_audioSystem.AddAudio("Laser", "audio/Laser.wav");
	meow::g_audioSystem.AddAudio("Explosion", "audio/Explosion.wav");
	meow::g_audioSystem.AddAudio("RocketBlast", "audio/RocketBlast2.wav");
	meow::g_audioSystem.AddAudio("HealthPickup", "audio/Health.wav");
	meow::g_audioSystem.AddAudio("Music", "audio/Music.wav");
	meow::g_audioSystem.AddAudio("GameOver", "audio/GameOver.wav");

	meow::g_audioSystem.PlayOneShot("Music", true);

	m_scene = std::make_unique<meow::Scene>();
	m_scene->Load("json/scene.json");
	m_scene->Initialize();

	m_scene->GetActorByName("EndScreen")->active = false;
	m_scene->GetActorByName("Title")->active = true;
	m_scene->GetActorByName("Score")->active = false;
	m_scene->GetActorByName("Health")->active = false;
	m_scene->GetActorByName("LivesSprite1")->active = false;
	m_scene->GetActorByName("LivesSprite2")->active = false;
	m_scene->GetActorByName("LivesSprite3")->active = false;

	EVENT_SUBSCRIBE("AddPoints", SpaceGame::OnAddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", SpaceGame::OnPlayerDead);

	

	return true;
}

void SpaceGame::Shutdown()
{
}

void SpaceGame::Update(float dt)
{
	switch (m_state)
	{
	case SpaceGame::eState::Title:
		m_scene->GetActorByName("Title")->active = true;
		m_scene->GetActorByName("Health")->active = false;
		m_scene->GetActorByName("Score")->active = false;
		m_scene->GetActorByName("EndScreen")->active = false;
		if (meow::g_inputSystem.GetMouseButtonDown(0) && !meow::g_inputSystem.GetMousePreviousButtonDown(0)) {

			m_state = eState::StartGame;
			
		}
		break;
	case SpaceGame::eState::StartGame:
		m_score = 0;
		m_lives = 3;
		m_state = eState::StartLevel;
		m_spawnTime = 3.0f;
		enemyCounter = 0;
		m_scene->GetActorByName("LivesSprite1")->active = true;
		m_scene->GetActorByName("LivesSprite2")->active = true;
		m_scene->GetActorByName("LivesSprite3")->active = true;
		m_scene->GetActorByName("EndScreen")->active = false;
	//	m_LivesText->Create(meow::g_renderer, "LIVES: " + std::to_string(m_lives), meow::Color{ 1, 1, 1, 1 });
		break;
	case SpaceGame::eState::StartLevel:
	m_scene->GetActorByName("Title")->active = false;
	m_scene->GetActorByName("Score")->active = true;
	m_scene->GetActorByName("Health")->active = true;

		m_spawnTime = 3.0f;
	{
		m_state = eState::Game;

		auto player = INSTANTIATE(Player, "Player");
 		player->transform = meow::Transform{ { 400, 300 }, 0, 3 };
		player->m_game = this;
		player->Initialize();
		m_scene->Add(std::move(player));


	}
		break;
	case SpaceGame::eState::Game:
		m_spawnTimer += dt;
		if (m_spawnTimer >= m_spawnTime) {
			m_spawnTimer = 0;

			enemyCounter++;

		auto enemy = INSTANTIATE(Enemy, "Enemy");
		enemy->transform = meow::Transform{ { meow::random(800), meow::random(600) }, meow::randomf(meow::TwoPi), 1.0f };
		enemy->m_game = this;
		if (enemyCounter > 10)
		{
			enemy->enemyUpgrade = true;
		}
		enemy->Initialize();
		m_scene->Add(std::move(enemy));

		if (m_spawnTime > 0.70f) {
			m_spawnTime -= 0.10f;
		}
		}
		m_HealthPadcd += dt;
		if (m_HealthPadTimer <= m_HealthPadcd)
		{
			m_HealthPadcd = 0;
			auto healthPad = INSTANTIATE(HealthPad, "HealthPad");
			healthPad->transform = meow::Transform{ {meow::random(meow::g_renderer.GetWidth()),meow::random(meow::g_renderer.GetHeight())}, 0, 1.5f };
			healthPad->Initialize();
			m_scene->Add(std::move(healthPad));

		}


		break;
	case eState::PlayerDeadStart:
		m_stateTimer = 3;
		

		if (m_lives <= 0) m_state = eState::GameOver;
		else m_state = eState::PlayerDead;
		break;
	case SpaceGame::eState::PlayerDead:
		m_stateTimer -= dt;
		m_scene->RemoveAll(false);
		if (m_stateTimer <= 0) {
			m_state = eState::StartLevel;
		}
		break;
	case SpaceGame::eState::GameOver:
		m_stateTimer -= dt;
		meow::g_audioSystem.PlayOneShot("GameOver");
		if (m_stateTimer <= 0) {
			if (meow::g_inputSystem.GetMouseButtonDown(0) && !meow::g_inputSystem.GetMousePreviousButtonDown(0)) {
				m_state = eState::Title;
				m_scene->RemoveAll(false);
			}
		}
		break;
	default:
		break;
	}
	m_scene->Update(dt);
}

void SpaceGame::Draw(meow::Renderer& renderer)
{
	m_scene->Draw(renderer);
	if (m_state == eState::Title) {

		
	}
	if (m_state == eState::GameOver) {
	
		m_scene->GetActorByName("EndScreen")->active = true;
		m_scene->GetActorByName("Score")->active = false;
		m_scene->GetActorByName("Health")->active = false;
		m_scene->GetActorByName("Enemy")->active = false;
	}
	

}

void SpaceGame::OnAddPoints(const meow::Event& event)
{
	m_score += std::get<int>(event.data);
	m_scene->GetActorByName("Score")->GetComponent<meow::TextRenderComponent>()->text = "Score: " + std::to_string((m_score));
}

void SpaceGame::OnPlayerDead(const meow::Event& event)
{
	m_scene->GetActorByName("LivesSprite"+std::to_string(m_lives))->active = false;
	m_lives--;
	m_state = eState::PlayerDeadStart;
}
