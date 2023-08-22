#include "Bullet.h"
#include "Framework/Emitter.h"


void Bullet::Update(float dt)
{
	Actor::Update(dt);

	meow::Vector2 forward = meow::vec2{ 0,-1 }.Rotate(transform.rotation);
	transform.position += forward * m_speed * 1 * dt;
	if (tag != "Player") {
		transform.position.x = meow::Wrap(transform.position.x, (float)meow::g_renderer.GetWidth());
		transform.position.y = meow::Wrap(transform.position.y, (float)meow::g_renderer.GetHeight());
	}
}
bool Bullet::Initialize()
{
	return false;
}
void Bullet::onCollision(Actor* other)
{


	if (other->tag != tag) {
		m_destroyed = true;
	}
}