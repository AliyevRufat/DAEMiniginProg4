#include "MiniginPCH.h"
#include "HealthComponent.h"

HealthComponent::HealthComponent(const unsigned int& health)
	: m_Lives{ health }
	, m_Dead{ false }
{
}

void HealthComponent::SetLives(const unsigned int& health)
{
	m_Lives = health;
}

void HealthComponent::Die()
{
	m_Dead = true;

	if ((int)m_Lives - 1 < 0)
		return;

	m_Lives--;
	m_pGameObject->Notify(Event::ActorDied);
}

void HealthComponent::Respawn()
{
	m_Dead = false;
}

const unsigned int& HealthComponent::GetLives() const
{
	return m_Lives;
}

bool HealthComponent::IsAlive() const
{
	return (!m_Dead);
}