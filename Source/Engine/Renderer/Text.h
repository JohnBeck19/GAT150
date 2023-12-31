#pragma once
#include "Renderer.h"
#include "Font.h"
#include "Core/Color.h"
#include <string>
#include <memory>
namespace meow
{
	class Text
	{
	public:
		Text() = default;
		Text(std::shared_ptr<Font> font) : m_font{ font } { }
		~Text();
		void Create(Renderer& renderer, const std::string& text, const Color& color);
		void Draw(Renderer& renderer, int x, int y);
		void Draw(Renderer& renderer, const class Transform& transform);
		friend class Font;
		friend class Renderer;

	private:
		std::shared_ptr<Font> m_font;
		struct SDL_Texture* m_texture = nullptr;
	};
}