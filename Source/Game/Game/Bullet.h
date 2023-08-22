#pragma once
#include "Framework/Actor.h"
class Bullet : public meow::Actor
{
public:
	Bullet(float speed, const meow::Transform& transform) :
		Actor{ transform},
		m_speed{ speed }
	{
		lifespan = 2.0f; 
		id = "bullet";
	}
	void onCollision(Actor* other) override;
	void Update(float dt) override;
	bool Initialize() override;
		
private:
	float m_speed = 0;

};