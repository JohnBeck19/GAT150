#include "Actor.h"
#include "RenderComponent.h"

meow::CLASS_DEFINITION(Actor)

meow::Actor::Actor(const Actor& other)
{
	name = other.name;
	tag = other.tag;
	lifespan = other.lifespan;
	transform = other.transform;
	m_scene = other.m_scene;
	m_game = other.m_game;
}

bool meow::Actor::Initialize()
{
	for (auto& component : m_components)
	{
		component->Initialize();
	}

	return true;
}

void meow::Actor::OnDestroy()
{
	for (auto& component : m_components)
	{
		component->OnDestroy();
	}
}

void meow::Actor::Update(float dt)
{
	if (lifespan != -1.0f) {
		lifespan -= dt;
		m_destroyed = (lifespan <= 0);
		
	}

	for (auto& component : m_components)
	{
		component->Update(dt);
	}
}

void meow::Actor::Draw(meow::Renderer& renderer)
{
	
	for (auto& component : m_components) {
		RenderComponent* renderComponent = dynamic_cast<RenderComponent*>(component.get());
		if (renderComponent)
		{
			renderComponent->Draw(renderer);
		}
	}

}

void meow::Actor::AddComponent(std::unique_ptr<meow::Component> component)
{
	component->m_owner = this;
	m_components.push_back(std::move(component));
}

void meow::Actor::Read(const json_t& value)
{
	Object::Read(value);

	READ_DATA(value, tag);
	READ_DATA(value, lifespan);
	READ_DATA(value, persistent);
	READ_DATA(value, prototype);

	if (HAS_DATA(value, transform)) transform.Read(GET_DATA(value,transform));

	if (HAS_DATA(value, components) && GET_DATA(value, components).IsArray()) {
		for (auto& componentValue : GET_DATA(value, components).GetArray())
		{
			std::string type;
			READ_DATA(componentValue, type);

			auto component = CREATE_CLASS_BASE(Component,type);
			component->Read(componentValue);

			AddComponent(std::move(component));
		}
	}

}
