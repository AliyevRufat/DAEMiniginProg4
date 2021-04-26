#pragma once
#include "AudioService.h"
#include "SoundEffect.h"
#include "SoundStream.h"
#include <map>

class ConsoleAudioService final : public AudioService
{
public:

	ConsoleAudioService();
	~ConsoleAudioService();

	virtual void QueueSound(const SoundIds& soundId, bool isEffect, int volume = 1) override;

	virtual void StopSound(const SoundIds& soundId, bool isEffect) override;

	virtual void AddSound(const SoundIds& soundId, const std::string& filePath, bool isEffect) override;

	virtual void StopAllSounds() override;

	virtual void Update() override;

protected:
	virtual void PlaySound(const SoundIds& soundId, int volume, bool isEffect) override;
private:
	std::map<SoundIds, SoundEffect*> m_SoundEffectMap;
	std::map<SoundIds, SoundStream*> m_SoundStreamMap;

	std::atomic_bool m_Playing;
	std::mutex m_Mutex;
	std::condition_variable m_ActiveQueue{};
	std::queue<Sound> m_SoundQueue;
};
