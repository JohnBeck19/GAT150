#include "Model.h"
#include <sstream>


namespace meow {

	bool Model::Create(std::string filename, ...) {
	
		return load(filename);
	}

	bool meow::Model::load(const std::string& filename)
	{
		std::string buffer;
		readFile(filename, buffer);


		std::istringstream stream(buffer);

		stream >> m_color;
		std::string line;
		std::getline(stream,line);
		
		int numPoints = std::stoi(line);
		for (int i = 0; i < numPoints; i++)
		{
			vec2 point;
			stream >> point;
			m_points.push_back(point);
		
		}
		GetRadius();
		return true;
	}
	void meow::Model::Draw(meow::Renderer& renderer, const vec2& position, float rotation , float scale)
	{
		if (m_points.empty()) return;

		renderer.SetColor(Color::ToInt(m_color.r), Color::ToInt(m_color.g), Color::ToInt(m_color.b), Color::ToInt(m_color.a));
		for (unsigned int i = 0; i < m_points.size()-1; i++)
		{
			vec2 p1 = (m_points[i] * scale).Rotate(rotation)+ position;
			vec2 p2 = (m_points[i+1] * scale).Rotate(rotation) +position;
			renderer.DrawLine(p1.x, p1.y, p2.x, p2.y);
		}
		
	}

	void Model::Draw(Renderer& renderer, const Transform& transform)
	{
		if (m_points.empty()) return;

		mat3 mx = transform.GetMatrix();

		renderer.SetColor(Color::ToInt(m_color.r), Color::ToInt(m_color.g), Color::ToInt(m_color.b), Color::ToInt(m_color.a));
		for (unsigned int i = 0; i < m_points.size() - 1; i++)
		{
			vec2 p1 = (mx * m_points[i]) + transform.position;
			vec2 p2 = (mx * m_points[i + 1]) + transform.position;
			renderer.DrawLine(p1.x, p1.y, p2.x, p2.y);
		}
	}

	float Model::GetRadius()
	{
		if (m_radius != 0) return m_radius;
			//if length is bigger radius gets replaced 
		for (auto point : m_points) {
			float length = point.Length();
			m_radius = Max(m_radius, length);

		}
		return m_radius;
	}

}