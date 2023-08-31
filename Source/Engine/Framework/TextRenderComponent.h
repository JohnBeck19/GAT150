#pragma once
#include "RenderComponent.h"
#include "Renderer/Text.h"
namespace meow
{
	class TextRenderComponent : public RenderComponent
	{
	public:
		CLASS_DECLARATION(TextRenderComponent)

		TextRenderComponent() = default;
		TextRenderComponent(const TextRenderComponent& other);

		bool Initialize() override;
		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;
		void SetText(const std::string& string);

	public:
		std::string text;
		std::string fontName;
		int fontSize = 0;
		Color color{1,1,1,1};
		bool m_changed = true;
	private:
		
		std::unique_ptr<Text> m_text;
	};
}