#include "Bullet.h"
#include "Framework/Framework.h"

namespace meow {
	CLASS_DEFINITION(Bullet)

	void Bullet::Update(float dt)
	{
		Actor::Update(dt);

		meow::Vector2 forward = meow::vec2{ 0,-1 }.Rotate(transform.rotation);
		m_physicsComponent->SetVelocity(forward* speed);
		if (tag != "Player") {
			transform.position.x = meow::Wrap(transform.position.x, (float)meow::g_renderer.GetWidth());
			transform.position.y = meow::Wrap(transform.position.y, (float)meow::g_renderer.GetHeight());
		}
	}
	bool Bullet::Initialize()
	{
		 Actor::Initialize();
		 m_physicsComponent = GetComponent<meow::PhysicsComponent>();
		 auto CollisionComponent = GetComponent<meow::CollisionComponent>();
		 if (CollisionComponent) {
			 auto renderComponent = GetComponent<meow::RenderComponent>();
			 if (renderComponent) {

				 float scale = transform.scale;
				 CollisionComponent->m_radius = renderComponent->GetRadius();
			 }
		 }
		 return true;
	}
	void Bullet::Read(const json_t& value)
	{
		Actor::Read(value);
		READ_DATA(value, speed);

	}
	void Bullet::OnCollisionEnter(Actor* other)
	{
		if (other->name == "Player") {
			destroyed = true;
		}
	}

}
