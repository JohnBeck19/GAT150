#pragma once
#include "RenderComponent.h"
#include "Renderer/Texture.h"
#include "Framework/Factory.h"
#include "Core/MathRect.h"
namespace meow {
	class SpriteComponent : public RenderComponent
	{
	public:
		CLASS_DECLARATION(SpriteComponent)
		bool Initialize() override;
		void Update(float dt) override;
		void Draw(class Renderer& renderer);


		virtual float GetRadius() { return m_texture->GetSize().Length()*0.5f; }


	public:
		MathRect source;
		std::string textureName;
		res_t<Texture> m_texture;
	};

}