#include "Bullet.h"
#include "Framework/Emitter.h"


void Bullet::Update(float dt)
{
	Actor::Update(dt);

	meow::Vector2 forward = meow::vec2{ 0,-1 }.Rotate(m_transform.rotation);
	m_transform.position += forward * m_speed * 1 * dt;
	if (m_tag != "Player") {
		m_transform.position.x = meow::Wrap(m_transform.position.x, (float)meow::g_renderer.GetWidth());
		m_transform.position.y = meow::Wrap(m_transform.position.y, (float)meow::g_renderer.GetHeight());
	}
}
void Bullet::onCollision(Actor* other)
{


	if (other->m_tag != m_tag) {
		m_destroyed = true;
	}
}