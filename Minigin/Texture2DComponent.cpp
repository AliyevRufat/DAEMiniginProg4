#include "MiniginPCH.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"

Texture2DComponent::Texture2DComponent(const std::string& filename)
	:m_Texture2D(dae::ResourceManager::GetInstance().LoadTexture(filename))
{
}

std::shared_ptr<dae::Texture2D> Texture2DComponent::GetTexture2D() const
{
	return m_Texture2D;
}

void Texture2DComponent::Render()
{
	if (!m_IsInitialized && m_pGameObject->GetComponent<TransformComponent>()) // doonce
	{
		m_IsInitialized = true;
		m_Position = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	}
	//
	dae::Renderer::GetInstance().RenderTexture(*m_Texture2D, m_Position.x, m_Position.y);
};