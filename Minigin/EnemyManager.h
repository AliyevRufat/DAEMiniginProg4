#pragma once
#include "Singleton.h"
#include <vector>
#include "GameObject.h"
#include <SDL.h>
#include "Scene.h"

class EnemyManager : public dae::Singleton<EnemyManager>
{
public:

	enum class EnemyType
	{
		Coily,
		Sam,
		Slick,
		Ugg,
		WrongWay
	};

	void Update();

	void SpawnEnemy(EnemyType enemyType, dae::Scene::GameMode gameMode = dae::Scene::GameMode::SinglePlayer);

	void DeleteFallenEnemies();
	void DeleteAllEnemies();
	//getters
	float GetEnemySpeedMultiplier() const;
	//setters
	void SetParametersAccordingToTheLevel(dae::Scene::Level currentLevel);
	void SetWindowSurface(SDL_Surface* windowSurface);
	void SetPlayers(const std::shared_ptr<GameObject>& pPlayer, const std::shared_ptr<GameObject>& pPlayer2 = nullptr);
	void SetLevelObject(const std::shared_ptr<GameObject>& levelObject);

private:
	friend class dae::Singleton<EnemyManager>;
	EnemyManager() = default;
	//Methods
	void SpawnCoily(dae::Scene::GameMode gameMode);
	void SpawnSamOrSlick(EnemyType enemyType);
	void SpawnWrongWayOrUgg(EnemyType enemyType);
	//DataMembers
	float m_SpawnTimerCoily = 0.f;
	float m_SpawnTimeCoily = 10.f;

	float m_SpawnTimerSamSlick = 0.f;
	float m_SpawnTimeSamSlick = 15.f;

	float m_SpawnTimerWrongWayUgg = 0.f;
	float m_SpawnTimeWrongWayUgg = 8.f;
	//
	bool m_CoilySpawned = false;
	bool m_SamOrSlickSpawned = false;
	//
	int m_WrongWayUggAmount = 0;
	const int m_MaxWrongWayUggAmount = 3;
	//
	const int m_EnemyWidth = 15;
	const int m_EnemyHeight = 16;
	float m_SpeedMultiplier = 1.0f;
	//
	SDL_Surface* m_pWindowSurface = nullptr;
	std::shared_ptr<GameObject> m_SpPlayer = nullptr;
	std::shared_ptr<GameObject> m_SpPlayer2 = nullptr;
	std::shared_ptr<GameObject> m_LevelObject = nullptr;
	//
	dae::Scene::Level m_CurrentLevel;
	std::vector<std::shared_ptr<GameObject>> m_SpEnemies;
};
