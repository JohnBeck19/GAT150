#pragma once
#include "Core/Core.h"
#include <vector>
#include "Renderer/Renderer.h"
#include "Framework/Resource.h"

namespace meow {

	class Model : public Resource {

	public:
		Model() = default;
		Model(const std::vector<vec2>& points) : m_points{ points } {};

		bool Create(std::string filename, ...);
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
