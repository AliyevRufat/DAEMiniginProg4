#include "pch.h"
#include "Locator.h"

AudioService* Locator::m_pService = nullptr;
NullAudio Locator::m_NullService{};

void Locator::Initialize()
{
	m_pService = &m_NullService;
}

AudioService& Locator::GetAudio()
{
	if (m_pService)
	{
		return *m_pService;
	}
	else
	{
		return m_NullService;
	}
}

void Locator::FreeResources()
{
	if (m_pService)
	{
		delete m_pService;
	}
}

void Locator::Provide(AudioService* pService)
{
	if (pService == NULL)
	{
		m_pService = &m_NullService;
	}
	else
	{
		m_pService = pService;
	}
}