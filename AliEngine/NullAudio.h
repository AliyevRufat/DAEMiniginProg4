#pragma once
#include "AudioService.h"

class NullAudio final : public AudioService
{
public:
	NullAudio() = default;
	~NullAudio() override = default;
	virtual void QueueSound(const SoundIds&, bool, int = 1) override {};
	virtual void StopSound(const SoundIds&, bool) override {};
	virtual void AddSound(const SoundIds&, const std::string&, bool)override {};
	virtual void StopAllSounds() override {};
	virtual void Update() override {};

protected:
	virtual void PlaySound(const SoundIds&, int, bool) override {};
};