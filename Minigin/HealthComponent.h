#pragma once
#include "ComponentIncludes.h"

class HealthComponent : public BaseComponent
{
public:
	HealthComponent(const unsigned int& health);

	const unsigned int& GetLives() const;
	void SetLives(const unsigned int&);
	void Die();
private:
	unsigned int m_Lives;
};