#include "HealthPad.h"
#include "Framework/Framework.h"
namespace meow {
	CLASS_DEFINITION(HealthPad)

	void HealthPad::Update(float dt)
	{
		Actor::Update(dt);

	}
	bool HealthPad::Initialize()
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
	void HealthPad::Read(const json_t& value)
	{
		Actor::Read(value);
		READ_DATA(value,HPAmount)
	}
	void HealthPad::OnCollisionEnter(Actor* actor)
	{
		if (actor->name == "Player") {
			destroyed = true;
		}

	}

}
