#include "pch.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/GameObject.h"
#include <algorithm>
#include <string>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name(name)
	, m_SpCurrentLevel{}
	, m_SpLevels{}
	, m_SpObjects{}
	, m_SpPlayers{}
	, m_CurrentGameMode{}
	, m_CurrentLevel{ Level::FirstLevel }
	, m_AreAllObjectsActive{ true }
{
}

void Scene::Add(const std::shared_ptr<SceneObject>& spObject)
{
	m_SpObjects.push_back(spObject);
}

void Scene::Update()
{
	for (size_t i = 0; i < m_SpObjects.size(); i++)
	{
		m_SpObjects[i]->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_SpObjects)
	{
		object->Render();
	}
}

const std::string& Scene::GetName() const
{
	return m_Name;
}

std::shared_ptr<SceneObject> Scene::GetObjectByName(const std::string& name) const
{
	for (const auto& object : m_SpObjects)
	{
		if (object->GetName().compare(name) == 0)
		{
			return object;
		}
	}
	return nullptr;
}

void Scene::AddPlayer(const std::shared_ptr<GameObject>& spPlayer)
{
	m_SpPlayers.push_back(spPlayer);
}

std::shared_ptr<GameObject> Scene::GetPlayer(int index)
{
	if (m_SpPlayers.size() <= (unsigned)index)
	{
		return nullptr;
	}

	return m_SpPlayers[index];
}

void Scene::SetCurrentLevel(const std::shared_ptr<GameObject>& level)
{
	m_SpCurrentLevel = level;
}

std::shared_ptr<GameObject> Scene::GetCurrentLevel() const
{
	return m_SpCurrentLevel;
}

void Scene::AddLevel(const std::shared_ptr<GameObject>& level)
{
	m_SpLevels.push_back(level);
	m_SpCurrentLevel = level;
}

std::shared_ptr<GameObject> Scene::GetLevel(int index) const
{
	return m_SpLevels[index];
}

float dae::Scene::GetSceneScale() const
{
	return m_SceneScale;
}

void dae::Scene::DeleteMarkedObjects()
{
	for (size_t i = 0; i < m_SpObjects.size(); i++)
	{
		if (m_SpObjects[i]->GetMarkForDelete())
		{
			m_SpObjects.erase(std::remove(m_SpObjects.begin(), m_SpObjects.end(), *(m_SpObjects.begin() + i)));
			--i;
		}
	}
}

bool dae::Scene::AreAllObjectsActive() const
{
	return m_AreAllObjectsActive;
}

void dae::Scene::SetObjectsIsActive(bool isActive)
{
	for (auto& object : m_SpObjects)
	{
		if (object->GetName() != "Cube" && object->GetName() != "Pyramid")
		{
			object->SetIsActive(isActive);
		}
	}

	for (auto& player : m_SpPlayers)
	{
		player->SetIsActive(isActive);
	}

	m_AreAllObjectsActive = !m_AreAllObjectsActive;
}

void dae::Scene::ClearScene()
{
	m_SpObjects.clear();
	m_SpPlayers.clear();
	m_SpLevels.clear();
	m_SpCurrentLevel = nullptr;
	m_CurrentLevel = Level::FirstLevel;
}