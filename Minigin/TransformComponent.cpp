#include "MiniginPCH.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(const glm::vec3& pos)
{
	m_Transform.SetPosition(pos.x, pos.y, pos.z);
}

dae::Transform TransformComponent::GetTransform() const
{
	return m_Transform;
}

void TransformComponent::SetPosition(const glm::vec3& position)
{
	m_Transform.SetPosition(position.x, position.y, position.z);
}