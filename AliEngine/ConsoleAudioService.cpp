#include "pch.h"
#include "../AliEngine/ConsoleAudioService.h"

ConsoleAudioService::ConsoleAudioService()
	:m_SoundEffectMap{}
	, m_SoundStreamMap{}
{
	m_Playing.store(true);
}

ConsoleAudioService::~ConsoleAudioService()
{
	m_Playing.store(false);
	m_ActiveQueue.notify_one();
	//delete pointers in the maps
	for (std::map<SoundIds, SoundEffect*>::iterator itr = m_SoundEffectMap.begin(); itr != m_SoundEffectMap.end(); itr++)
	{
		delete itr->second;
	}
	for (std::map<SoundIds, SoundStream*>::iterator itr = m_SoundStreamMap.begin(); itr != m_SoundStreamMap.end(); itr++)
	{
		delete itr->second;
	}
}

void ConsoleAudioService::PlaySound(const SoundIds& soundId, int volume, bool isEffect)
{
	if (isEffect)
	{
		if (m_SoundEffectMap.find(soundId) != m_SoundEffectMap.end())
		{
			m_SoundEffectMap.at(soundId)->SetVolume(volume);
			m_SoundEffectMap.at(soundId)->Play(0);
		}
	}
	else
	{
		if (m_SoundStreamMap.find(soundId) != m_SoundStreamMap.end())
		{
			m_SoundStreamMap.at(soundId)->SetVolume(volume);
			m_SoundStreamMap.at(soundId)->Play(0);
		}
	}
}

void ConsoleAudioService::QueueSound(const SoundIds& soundId, bool isEffect, int)
{
	Sound sound{ soundId, isEffect,10 };
	std::lock_guard<std::mutex> mLock{ m_Mutex };
	m_SoundQueue.push(sound);
	m_ActiveQueue.notify_one();
}

void ConsoleAudioService::StopSound(const SoundIds& soundId, bool isEffect)
{
	if (isEffect)
	{
		if (m_SoundEffectMap.find(soundId) != m_SoundEffectMap.end())
		{
			m_SoundEffectMap.at(soundId)->PauseAll();
		}
	}
	else
	{
		if (m_SoundStreamMap.find(soundId) != m_SoundStreamMap.end())
		{
			m_SoundStreamMap.at(soundId)->Pause();
		}
	}
}

void ConsoleAudioService::AddSound(const SoundIds& soundId, const std::string& filePath, bool isEffect)
{
	if (isEffect)
	{
		m_SoundEffectMap.insert(std::pair<SoundIds, SoundEffect*>(soundId, new SoundEffect(filePath)));
	}
	else
	{
		m_SoundStreamMap.insert(std::pair<SoundIds, SoundStream*>(soundId, new SoundStream(filePath)));
	}
}

void ConsoleAudioService::StopAllSounds()
{
	SoundStream::Pause();
}

void ConsoleAudioService::Update()
{
	while (m_Playing.load())
	{
		while (!m_SoundQueue.empty())
		{
			std::unique_lock<std::mutex> mLock{ m_Mutex };
			Sound ps = m_SoundQueue.front();
			m_SoundQueue.pop();

			PlaySound(ps.soundId, ps.volume, ps.isEffect);
		}

		std::unique_lock<std::mutex> guard{ m_Mutex };
		m_ActiveQueue.wait(guard);
	}
}