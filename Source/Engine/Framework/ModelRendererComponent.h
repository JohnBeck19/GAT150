#pragma once
#include "RenderComponent.h"
#include "Renderer/Texture.h"
#include "Renderer/Model.h"


namespace meow {
	class ModelRenderComponent : public RenderComponent
	{
	public:
		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;

		//virtual void GetRadius() override { return 0.0f; }
	public:
		res_t<Model> m_model;
	};

}