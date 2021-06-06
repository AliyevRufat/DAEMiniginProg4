#pragma once
#include "Singleton.h"
#include <string>
#include <memory>
#include <vector>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void ClearScene(std::shared_ptr<dae::Scene> spScene);

		void Update();
		void Render();

		std::shared_ptr<Scene> GetSceneByName(const std::string& n) const;
		std::shared_ptr<Scene> GetCurrentScene();
		void SetCurrentScene(const std::shared_ptr<Scene>& spScene);

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_SpScenes;
		std::shared_ptr<Scene> m_SpCurrentScene;
	};
}
