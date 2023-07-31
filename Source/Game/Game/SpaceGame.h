#pragma once
#include "Framework/Game.h"
#include "Renderer/Text.h"

class SpaceGame : public meow::Game {

public:
	enum class eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		PlayerDead,
		PlayerDeadStart,
		GameOver
	};
public:

	virtual bool Initialize() override;

	virtual void Shutdown() override;

	virtual void Update(float dt) override;

	virtual void Draw(meow::Renderer& renderer) override;

	void SetState(eState state) { m_state = state; }

	std::unique_ptr<meow::Text> m_HealthText;
private:
	eState m_state = eState::Title;
	float m_spawnTimer = 0;
	float m_spawnTime = 3.0f;
	float m_stateTimer = 0;
	float m_HealthPadTimer = 15.0f;
	float m_HealthPadcd = 0;
	std::shared_ptr<meow::Font> m_font;
	std::unique_ptr<meow::Text> m_ScoreText;
	std::unique_ptr<meow::Text> m_TitleText;
	std::unique_ptr<meow::Text> m_LivesText;
	std::unique_ptr<meow::Text> m_GameOverText;

};