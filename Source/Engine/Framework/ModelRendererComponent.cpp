#include "ModelRendererComponent.h"
#include "Framework/Actor.h"
namespace meow
{

	void ModelRenderComponent::Update(float dt)
	{
		
	}

	void ModelRenderComponent::Draw(Renderer& renderer)
	{
		m_model->Draw(renderer, m_owner->transform);
	}

}