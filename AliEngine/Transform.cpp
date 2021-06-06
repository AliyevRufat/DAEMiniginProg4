#include "pch.h"
#include "../AliEngine/Transform.h"

void dae::Transform::SetPosition(const float x, const float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void dae::Transform::SetScale(const float x, const float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
}