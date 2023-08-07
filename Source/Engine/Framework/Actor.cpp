#include "Actor.h"
#include "RenderComponent.h"

void meow::Actor::Update(float dt)
{
	if (m_lifespan != -1.0f) {
		m_lifespan -= dt;
		m_destroyed = (m_lifespan <= 0);
		
	}
	m_transform.position += m_velocity * dt;
	m_velocity *= std::pow(1.0f - m_damping, dt);
}

void meow::Actor::Draw(meow::Renderer& renderer)
{
	//m_model->Draw(renderer, m_transform);
	for (auto& component : m_components) {
		if (dynamic_cast<RenderComponent*>(component.get()))
		{
			dynamic_cast<RenderComponent*>(component.get())->Draw(renderer);
		}
	}

}

void meow::Actor::AddComponent(std::unique_ptr<meow::Component> component)
{
	component->m_owner = this;
	m_components.push_back(std::move(component));
}
