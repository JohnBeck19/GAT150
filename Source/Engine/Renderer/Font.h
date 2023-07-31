#pragma once
#include <string>
struct _TTF_Font;
namespace meow
{
	class Font
	{
	public:
		Font() = default;
		Font(const std::string& filename, int fontSize);
		~Font();

		void Load(const std::string& filename, int fontSize);
		friend class Text;
	private:
		_TTF_Font* m_ttfFont = nullptr;
	};
}