#include "pch.h"
#include "../AliEngine/EngineTime.h"

float EngineTime::GetDeltaTime()
{
	return m_DeltaTime;
};

void EngineTime::SetDeltaTime(float deltaTime)
{
	m_DeltaTime = deltaTime;
};