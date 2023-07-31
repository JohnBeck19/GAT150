#pragma once
#include "Core/Core.h"
#include <vector>
#include "Renderer/Renderer.h"

namespace meow {

	class Model {

	public:
		Model() = default;
		Model(const std::vector<vec2>& points) : m_points{ points } {};
		bool load(const std::string& filename);

		void Draw(Renderer& renderer, const vec2& position, float rotation, float scale);
		void Draw(Renderer& renderer, const Transform& transform);

		float GetRadius();

		Color m_color;
		
	private:
		std::vector<vec2> m_points;
		
		float m_radius = 0;
	};

}
