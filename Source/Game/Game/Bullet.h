#pragma once
#include "Framework/Actor.h"
class Bullet : public meow::Actor
{
public:
	Bullet(float speed, const meow::Transform& transform, const std::shared_ptr<meow::Model> model) :
		Actor{ transform, model },
		m_speed{ speed }
	{
		m_lifespan = 2.0f; 
		m_id = "bullet";
	}
	void onCollision(Actor* other) override;
	void Update(float dt) override;


private:
	float m_speed = 0;

};