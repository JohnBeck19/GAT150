#pragma once
#include "Framework/Actor.h"
#include "Framework/PhysicsComponent.h"
namespace meow
{
	class Enemy : public Actor
	{
	public:
		CLASS_DECLARATION(Enemy)

		Enemy() = default;
		bool Initialize() override;
		void OnDestroy() override;
		void Update(float dt) override;

		void OnCollisionEnter(Actor* other) override;

	public:
		float speed = 0;
		float turnRate = 0;
		float fireRate = 0;
		float fireTimer = 0;
		bool enemyUpgrade = false;
		PhysicsComponent* m_physicsComponent = nullptr;
	};
}