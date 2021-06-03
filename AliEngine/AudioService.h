#pragma once
#include <thread>
#include <mutex>
#include <queue>

class AudioService
{
public:

	enum class SoundIds
	{
		AttackEffect,
		HurtEffect,
		JumpEffect,
		BackgroundStream,
		MainMenuStream
	};

	struct Sound
	{
		SoundIds soundId;
		bool isEffect;
		int volume;
	};

	AudioService() = default;
	virtual ~AudioService() = default;
	AudioService(const AudioService&) = delete;
	AudioService& operator=(const AudioService&) = delete;
	AudioService(AudioService&&) = delete;
	AudioService& operator= (AudioService&&) = delete;

	virtual void QueueSound(const SoundIds& soundId, bool isEffect, int = 1) = 0;
	virtual void StopSound(const SoundIds& soundId, bool isEffect) = 0;
	virtual void AddSound(const SoundIds& soundId, const std::string& filePath, bool isEffect) = 0;
	virtual void StopAllSounds() = 0;
	virtual void Update() = 0;

protected:
	virtual void PlaySound(const SoundIds& soundId, int volume, bool isEffect) = 0;
};
