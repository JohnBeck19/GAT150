#include "HealthPad.h"

void HealthPad::Update(float dt)
{
}

void HealthPad::onCollision(Actor* actor)
{
	if (actor->m_id == "Player") {
		m_destroyed = true;
	}

}
