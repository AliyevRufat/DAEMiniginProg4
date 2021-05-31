#include "MiniginPCH.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(const glm::vec2& pos, const glm::vec2& scale)
{
	m_Transform.SetPosition(pos.x, pos.y);
	m_Transform.SetScale(scale.x, scale.y);
}

dae::Transform TransformComponent::GetTransform() const
{
	return m_Transform;
}

void TransformComponent::SetPosition(const glm::vec2& position)
{
	m_Transform.SetPosition(position.x, position.y);
}

void TransformComponent::SetScale(const glm::vec2& scale)
{
	m_Transform.SetScale(scale.x, scale.y);
}