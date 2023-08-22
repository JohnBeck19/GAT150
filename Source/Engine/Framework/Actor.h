#pragma once
#include "Object.h"
#include "Renderer/Model.h"
#include "Core/Core.h"
#include "Component.h"
#include <memory>

namespace meow {
	class Actor : public Object
	{
	public:
		CLASS_DECLARATION(Actor)

		Actor() = default;
		Actor(const Actor& other);
		Actor(const Transform& transform) :
			transform{ transform }
		{}

		virtual bool Initialize() override;
		virtual void OnDestroy() override;


		virtual void Update(float dt);
		virtual void Draw(Renderer& renderer);
		
		template<typename T>
		T* GetComponent();

		void AddComponent(std::unique_ptr<Component> component);

		virtual void onCollision(Actor* other) {};

		float GetRadius() { return 30.0f; }



		class Scene* m_scene = nullptr;
		friend class Scene;
		Transform transform;
		std::string tag;
		std::string id;
		float lifespan = -1.0f;
		bool persistent = false; 
		bool prototype = false;
		friend class Game;
		class Game* m_game = nullptr;

		
	protected:

		std::vector <std::unique_ptr<Component>> m_components;
		bool m_destroyed = false;


	};

	template<typename T>
	inline T* Actor::GetComponent() {
		
		for (auto& component : m_components) {
			T* result = dynamic_cast<T*>(component.get());
			if (result) return result;
		}
		return nullptr;
		
	}
}