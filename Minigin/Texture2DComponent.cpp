#include "MiniginPCH.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"

Texture2DComponent::Texture2DComponent(const std::string& filename, float scale)
	:m_spTexture2D(dae::ResourceManager::GetInstance().LoadTexture(filename))
	, m_Scale{ scale }
	, m_SDLRect{}
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
	}
	//
	m_Position = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();

	if (m_pGameObject->GetComponent<AnimationComponent>())
	{
		m_SDLRect = m_pGameObject->GetComponent<AnimationComponent>()->Animate();

		dae::Renderer::GetInstance().RenderTexture(*m_spTexture2D, m_Position.x, m_Position.y, (float)m_SDLRect.w * m_Scale, (float)m_SDLRect.h * m_Scale, (float)m_SDLRect.x, (float)m_SDLRect.y, (float)m_SDLRect.w, (float)m_SDLRect.h);
	}
	else
	{
		int width = 0;
		int height = 0;
		SDL_QueryTexture(m_spTexture2D.get()->GetSDLTexture(), nullptr, nullptr, &width, &height);

		dae::Renderer::GetInstance().RenderTexture(*m_spTexture2D, m_Position.x, m_Position.y, (float)width * m_Scale, (float)height * m_Scale);
	}
};

const SDL_Rect& Texture2DComponent::GetSrcRect() const
{
	return m_SDLRect;
}