#include "pch.h"
#include <SDL.h>
#include "glm\vec2.hpp"
#include "Transform.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(const glm::vec2& pos, const glm::vec2& scale)
	:m_SDLRect{ SDL_Rect{(int)pos.x,(int)pos.y,(int)scale.x,(int)scale.y} }
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
	m_SDLRect.x = (int)position.x;
	m_SDLRect.y = (int)position.y;
	m_Transform.SetPosition(position.x, position.y);
}

void TransformComponent::SetScale(const glm::vec2& scale)
{
	m_SDLRect.w = (int)scale.x;
	m_SDLRect.h = (int)scale.y;
	m_Transform.SetScale(scale.x, scale.y);
}

const SDL_Rect& TransformComponent::GetRect()
{
	return m_SDLRect;
}