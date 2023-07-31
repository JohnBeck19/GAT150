#pragma once
#include "Core/Core.h"
#include "Renderer/Model.h"
#include <memory>
namespace meow {
	class Actor
	{
	public:
		Actor() = default;
		Actor(const Transform& transform,  std::shared_ptr<Model> model) :
			m_transform{ transform },
			m_model{ model }
		{}
		Actor(const Transform& transform) :
			m_transform{ transform }
		{}

		virtual void Update(float dt);
		virtual void Draw(Renderer& renderer);
		virtual void onCollision(Actor* other) {};

		float GetRadius() { return (m_model) ? m_model->GetRadius() * m_transform.scale : -10000; }

		void AddForce(vec2 force) { m_velocity += force; }
		void SetDamping(float damping) { m_damping = damping; }

		class Scene* m_scene = nullptr;
		friend class Scene;
		Transform m_transform;
		std::string m_tag;
		std::string m_id;
		float m_lifespan = -1.0f;

		friend class Game;
		class Game* m_game = nullptr;
	protected:

		bool m_destroyed = false;
		vec2 m_velocity;
		float m_damping = 0;
		std::shared_ptr<Model> m_model;




	};
}