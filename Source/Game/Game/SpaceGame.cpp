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
#include "Framework/CircleCollisionComponent.h"

bool SpaceGame::Initialize()
{
	
	m_ScoreText = std::make_unique<meow::Text>(GET_RESOURCE	(meow::Font, "Space age.ttf", 24));
	m_ScoreText->Create(meow::g_renderer, "SCORE: ", meow::Color{ 1, 1, 1, 1 });

	m_TitleText = std::make_unique<meow::Text>(GET_RESOURCE(meow::Font, "Space age.ttf", 24));
	m_TitleText->Create(meow::g_renderer, "ASTROIDS", meow::Color{ 1, 1, 1, 1 });

	m_GameOverText = std::make_unique<meow::Text>(GET_RESOURCE(meow::Font, "Space age.ttf", 24));
	m_GameOverText->Create(meow::g_renderer, "Game Over!", meow::Color{ 1, 1, 1, 1 });

	m_LivesText = std::make_unique<meow::Text>(GET_RESOURCE(meow::Font, "Space age.ttf", 24));
	
	m_HealthText = std::make_unique<meow::Text>(GET_RESOURCE(meow::Font,"Space age.ttf",24));

	meow::g_audioSystem.AddAudio("Laser", "Laser.wav");
	meow::g_audioSystem.AddAudio("Explosion", "Explosion.wav");
	
	m_scene = std::make_unique<meow::Scene>();
	m_scene->Load("scene.json");
	m_scene->Initialize();
	//m_scene->GetActorByName("Title")->active = true;

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
		
		if (meow::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE)) {
		
			m_state = eState::StartGame;
			//m_scene->GetActorByName("Background")->active = false;
		}
		break;
	case SpaceGame::eState::StartGame:
		m_score = 0;
		m_lives = 3;
		m_state = eState::StartLevel;
		m_LivesText->Create(meow::g_renderer, "LIVES: " + std::to_string(m_lives), meow::Color{ 1, 1, 1, 1 });
		break;
	case SpaceGame::eState::StartLevel:
		m_scene->RemoveAll(true);
		m_spawnTime = 3.0f;
	{
		//create player
		std::unique_ptr<Player> player = std::make_unique<Player>(10.0f, meow::Pi*1.25f, meow::Transform{{ 450,350 }, 0.0f, 3.0f});
		player->tag = "Player";
		player->m_game = this;
		m_state = eState::Game;
		m_HealthText->Create(meow::g_renderer, "HEALTH: " + std::to_string(player->getHP()), meow::Color{ 1, 1, 1, 1 });
		//create components
		auto spriteComponent = CREATE_CLASS(SpriteComponent);
		spriteComponent->m_texture = GET_RESOURCE(meow::Texture, "Rocket.png", meow::g_renderer);
		player->AddComponent(std::move(spriteComponent));	

		auto physicsComponent = CREATE_CLASS(EnginePhysicsComponent);
		physicsComponent->m_damping = 0.9f;
		player->AddComponent(std::move(physicsComponent));
		
		auto CollisionComponent= CREATE_CLASS(CircleCollisionComponent);
		CollisionComponent->m_radius = 30.0f;
		player->AddComponent(std::move(CollisionComponent));


		player->Initialize();
		m_scene->Add(std::move(player));

	}
		break;
	case SpaceGame::eState::Game:
		m_spawnTimer += dt;
		if (m_spawnTimer >= m_spawnTime) {
			m_spawnTimer = 0;
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(meow::randomf(75.0f, 150.0f), meow::Pi, meow::Transform{ {meow::random(meow::g_renderer.GetWidth()),meow::random(meow::g_renderer.GetHeight())}
		,meow::randomf(meow::TwoPi),1.5f });
		enemy->tag = "Enemy";
		enemy->m_game = this;
		std::unique_ptr<meow::SpriteComponent> component = std::make_unique<meow::SpriteComponent>();
		component->m_texture = GET_RESOURCE(meow::Texture, "Rocket.png", meow::g_renderer);
		enemy->AddComponent(std::move(component));

		auto CollisionComponent = std::make_unique<meow::CircleCollisionComponent>();
		enemy->AddComponent(std::move(CollisionComponent));

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
			std::unique_ptr<HealthPad> healthpad = std::make_unique<HealthPad>(20.0f, meow::Transform{ {meow::random(meow::g_renderer.GetWidth()),meow::random(meow::g_renderer.GetHeight())}
				,0,3.0f });
			healthpad->tag = "HealthPad";
			healthpad->m_game = this;
			m_scene->Add(std::move(healthpad));
		}


		break;
	case eState::PlayerDeadStart:
		m_stateTimer = 3;
		m_lives--;
		m_LivesText->Create(meow::g_renderer, "LIVES: " + std::to_string(m_lives), meow::Color{ 1, 1, 1, 1 });
		if (m_lives <= 0) m_state = eState::GameOver;
		else m_state = eState::PlayerDead;
		break;
	case SpaceGame::eState::PlayerDead:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0) {
			m_state = eState::StartLevel;
		}
		break;
	case SpaceGame::eState::GameOver:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0) {
			m_state = eState::Title;
		}
		break;
	default:
		break;
	}
	m_ScoreText->Create(meow::g_renderer, "Score: " + std::to_string(m_score), {1, 1, 1, 1});
	m_scene->Update(dt);
}

void SpaceGame::Draw(meow::Renderer& renderer)
{
	if (m_state == eState::Title) {
		m_TitleText->Draw(renderer, renderer.GetWidth()/2-75, renderer.GetHeight()/2-30);
		
	}
	if (m_state == eState::GameOver) {
		m_scene->RemoveAll(true);
		m_GameOverText->Draw(renderer, renderer.GetWidth()/2-75, renderer.GetHeight()/2-30);
	}
	m_LivesText->Draw(renderer, 40, renderer.GetHeight() - 40);
	m_ScoreText->Draw(renderer, 40, 40);
	m_HealthText->Draw(renderer, renderer.GetWidth() - 250, 40);
	m_scene->Draw(renderer);

}
