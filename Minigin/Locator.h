#pragma once
#include "AudioService.h"
#include "NullAudio.h"

class Locator final
{
public:
	static void Initialize();

	static AudioService& GetAudio();

	static void FreeResources();

	static void Provide(AudioService* service);

private:
	static AudioService* m_pService;
	static NullAudio m_NullService;
};
