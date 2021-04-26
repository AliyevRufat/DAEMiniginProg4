#include "MiniginPCH.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"

Texture2DComponent::Texture2DComponent(const std::string& filename)
	:m_spTexture2D(dae::ResourceManager::GetInstance().LoadTexture(filename))
{
}

std::shared_ptr<dae::Texture2D> Texture2DComponent::GetTexture2D() const
{
	return m_spTexture2D;
}

void Texture2DComponent::Render()
{
	if (!m_IsInitialized && m_pGameObject->GetComponent<TransformComponent>()) // doonce
	{
		m_IsInitialized = true;
		m_Position = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	}
	//

	if (m_pGameObject->GetComponent<AnimationComponent>())
	{
		SDL_Rect srcRect = m_pGameObject->GetComponent<AnimationComponent>()->Animate(AnimationState::JumpRight);

		dae::Renderer::GetInstance().RenderTexture(*m_spTexture2D, m_Position.x, m_Position.y, (float)srcRect.w * 4, (float)srcRect.h * 4, (float)srcRect.x, (float)srcRect.y, (float)srcRect.w, (float)srcRect.h);
	}
	else
	{
		dae::Renderer::GetInstance().RenderTexture(*m_spTexture2D, m_Position.x, m_Position.y);
	}
};