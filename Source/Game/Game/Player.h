#pragma once
#include "Framework/Actor.h"
#include "Framework/PhysicsComponent.h"
namespace meow {
	class Player : public meow::Actor
	{
	public:

		CLASS_DECLARATION(Player)

		bool Initialize() override;
		void Update(float dt) override;
		void OnCollisionEnter(Actor* actor) override;
		int getHP() { return (int)health; }
		void CreateBulletScaling(float offset);
	private:
		float speed = 0;
		float turnRate = 0;
		float health = 5;
		float maxHealth = 5;

		//super
		float superTimer = 5;
		float supercd = 3;
		int superburst = 3;
		bool superburstbool = false;
		float superburstdelay = .10f;

		//shot
		bool fireburst = false;
		float fireburstdelay = .10f;
		int fireburstcount = 1;
		int fireburstcountmax = 1;
		int LevelUp = 1000;
		float shotcd = 0.3f;
		float shotcdcooldown = 0.3f;
		meow::PhysicsComponent* m_physicsComponent = nullptr;
	};


}
