#pragma once
#include "Framework/Actor.h"


class Enemy : public meow::Actor
{
public:
	Enemy(float speed, float turnRate, const meow::Transform& transform, std::shared_ptr<meow::Model> model) :
		Actor{ transform, model },
		m_speed{ speed },
		m_turnRate{ turnRate }
	{
		m_fireRate = 2.0f;
		m_fireTimer = meow::randomf(1.0f,3.0f);
	}
	

	void Update(float dt) override;
	void onCollision(Actor* other) override;


private:
	float m_speed = 0;
	float m_turnRate = 0;

	float m_fireRate = 0;
	float m_fireTimer = 0;
};