#pragma once
#include "Framework/Actor.h"

class Player : public meow::Actor
{
public:
	Player(float speed, float turnRate, const meow::Transform& transform, const std::shared_ptr<meow::Model> model) :
		Actor{ transform, model },
		m_speed{ speed },
		m_turnRate{ turnRate }
	{
	m_id = "Player";
	}
	void Update(float dt) override;
	void onCollision(Actor* actor) override;
	int getHP() { return (int)m_health; }

private:
	float m_speed = 0;
	float m_turnRate = 0;
	float m_health = 30;
	float m_maxHealth = 30;
	float m_superTimer = 5;
	float m_supercd = 3;
	int m_burst = 3;
	bool m_burstbool = false;
	float m_burstdelay = .10f;
};