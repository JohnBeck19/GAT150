#pragma once
#include "Component.h"
namespace meow {

	class CollisionComponent : public Component
	{
	public:
		virtual bool CheckCollision(CollisionComponent* collision) = 0;

	public:
		float m_radius = 0.0f;



	};


}