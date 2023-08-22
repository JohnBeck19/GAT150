#pragma once
#include "CollisionComponent.h"

namespace meow {

	class CircleCollisionComponent : public CollisionComponent
	{
	public:

		CLASS_DECLARATION(CircleCollisionComponent)

		void Update(float dt);

		bool CheckCollision(CollisionComponent* collision);
	};
}