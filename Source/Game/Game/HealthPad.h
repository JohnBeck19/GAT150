#pragma once
#include "Framework/Actor.h"

class HealthPad : public meow::Actor
{
public:
	HealthPad(float HPAmount, const meow::Transform& transform, const std::shared_ptr<meow::Model> model) :
		Actor{ transform, model },
		m_HPAmount{ HPAmount}
	{}
	void Update(float dt) override;
	void onCollision(Actor* actor) override;
	float getHealthAmount() { return m_HPAmount; }

private:
	float m_HPAmount;

};