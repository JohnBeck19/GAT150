#pragma once
#include "Framework/Actor.h"
#include "Framework/PhysicsComponent.h"
namespace meow {
	class Bullet : public meow::Actor
	{
	public:

		CLASS_DECLARATION(Bullet)

		void OnCollisionEnter(Actor* other) override;
		void Update(float dt) override;
		bool Initialize() override;

	private:
		float speed = 0;
		meow::PhysicsComponent* m_physicsComponent = nullptr;
	};

}
