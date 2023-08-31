#pragma once
#include "Framework/Actor.h"
#include "Framework/PhysicsComponent.h"
namespace meow {
	class HealthPad : public meow::Actor
	{
	public:
		CLASS_DECLARATION(HealthPad)

		void Update(float dt) override;
		bool Initialize() override;
		void OnCollisionEnter(Actor* actor) override;
		float getHealthAmount() { return HPAmount; }

	private:
		float HPAmount;
		meow::PhysicsComponent* m_physicsComponent = nullptr;
	};


}
