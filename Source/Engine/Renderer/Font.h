#pragma once
#include "Framework/ResourceManager.h"
#include <string>
struct _TTF_Font;
namespace meow
{
	class Font : public Resource
	{
	public:
		Font() = default;
		Font(const std::string& filename, int fontSize);
		~Font();

		bool Load(const std::string& filename, int fontSize);

		// Inherited via Resource
		virtual bool Create(std::string filename, ...) override;

		friend class Text;
	private:
		_TTF_Font* m_ttfFont = nullptr;

	};
}