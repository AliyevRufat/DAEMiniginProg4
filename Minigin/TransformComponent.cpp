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