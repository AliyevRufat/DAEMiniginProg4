#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	m_SpCurrentScene->Update();
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