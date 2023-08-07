#pragma once

namespace meow
{
	class Component {
	public:
		virtual void Update(float dt) = 0;

		friend class Actor;
	protected:
		class Actor* m_owner = nullptr;

	};

}