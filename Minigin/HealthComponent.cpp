#include "MiniginPCH.h"
#include "HealthComponent.h"

HealthComponent::HealthComponent(const unsigned int& health)
	: m_Lives{ health }
{
}

void HealthComponent::SetLives(const unsigned int& health)
{
	m_Lives = health;
}

void HealthComponent::Die()
{
	if ((int)m_Lives - 1 < 0)
	{
		return;
	}

	m_Lives--;
	m_pGameObject->Notify(Event::ActorDied);
}

const unsigned int& HealthComponent::GetLives() const
{
	return m_Lives;
}