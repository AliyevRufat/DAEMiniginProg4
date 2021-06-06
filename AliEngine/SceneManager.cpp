#include "pch.h"
#include "../AliEngine/SceneManager.h"
#include "../AliEngine/Scene.h"

void dae::SceneManager::Update()
{
	m_SpCurrentScene->Update();

	for (size_t i = 0; i < m_SpScenes.size(); i++)
	{
		m_SpScenes[i]->DeleteMarkedObjects();
	}
}

void dae::SceneManager::Render()
{
	m_SpCurrentScene->Render();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto spScene = std::shared_ptr<Scene>(new Scene(name));
	m_SpScenes.push_back(spScene);

	m_SpCurrentScene = spScene;

	return *spScene;
}

void dae::SceneManager::ClearScene(std::shared_ptr<dae::Scene> spScene)
{
	auto sceneIt = std::find_if(m_SpScenes.begin(), m_SpScenes.end(), [&](auto scene) { return spScene == scene; });

	if (sceneIt != m_SpScenes.end())
	{
		sceneIt->get()->ClearScene();
	}
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetSceneByName(const std::string& n) const
{
	for (const auto& scene : m_SpScenes)
	{
		if (scene->GetName().compare(n) == 0)
			return scene;
	}
	return nullptr;
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetCurrentScene()
{
	return m_SpCurrentScene;
}

void dae::SceneManager::SetCurrentScene(const std::shared_ptr<Scene>& spScene)
{
	m_SpCurrentScene = spScene;
}