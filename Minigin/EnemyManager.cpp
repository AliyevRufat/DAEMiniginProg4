#include "MiniginPCH.h"
#include "EnemyManager.h"
#include "EnemyManager.h"
#include "../AliEngine/SceneManager.h"
#include "EnemyMovementComponent.h"
#include "CollisionDetectionManager.h"
#include "LivesObserver.h"
#include "Minigin.h"
#include "../AliEngine/TransformComponent.h"
#include "PlayerMovementComponent.h"
#include "../AliEngine/Texture2DComponent.h"
#include "PyramidComponent.h"
#include "EnemyMovementComponent.h"
#include "HealthComponent.h"
#include "GameStateManager.h"

void EnemyManager::Update()
{
	if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() == dae::Scene::GameMode::None || dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() == dae::Scene::GameMode::Versus)
	{
		return;
	}

	DeleteFallenEnemies();
	auto deltaTime = EngineTime::GetInstance().GetDeltaTime();
	if (!m_CoilySpawned)
	{
		m_SpawnTimerCoily += deltaTime;
	}
	if (!m_SamOrSlickSpawned)
	{
		m_SpawnTimerSamSlick += deltaTime;
	}
	m_SpawnTimerWrongWayUgg += deltaTime;

	const int randNr = rand() % 2;

	if (m_SpawnTimerCoily >= m_SpawnTimeCoily && !m_CoilySpawned && dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() != dae::Scene::GameMode::Versus)
	{
		SpawnEnemy(EnemyType::Coily);
		m_CoilySpawned = true;
		m_SpawnTimerCoily -= m_SpawnTimerCoily;
	}
	if (m_SpawnTimerWrongWayUgg >= m_SpawnTimeWrongWayUgg && m_WrongWayUggAmount <= m_MaxWrongWayUggAmount)
	{
		if (randNr == 0)
		{
			SpawnEnemy(EnemyType::WrongWay);
		}
		else
		{
			SpawnEnemy(EnemyType::Ugg);
		}
		++m_WrongWayUggAmount;
		m_SpawnTimerWrongWayUgg -= m_SpawnTimerWrongWayUgg;
	}
	if (m_SpawnTimerSamSlick >= m_SpawnTimeSamSlick)
	{
		if (randNr == 0)
		{
			SpawnEnemy(EnemyType::Sam);
		}
		else
		{
			SpawnEnemy(EnemyType::Slick);
		}
		m_SamOrSlickSpawned = true;
		m_SpawnTimerSamSlick -= m_SpawnTimerSamSlick;
	}
}

void EnemyManager::SpawnEnemy(EnemyType enemyType, dae::Scene::GameMode gameMode)
{
	if (enemyType == EnemyType::Coily)
	{
		SpawnCoily(gameMode);
	}
	else if (enemyType == EnemyType::Sam || enemyType == EnemyType::Slick)
	{
		SpawnSamOrSlick(enemyType);
	}
	else if (enemyType == EnemyType::WrongWay || enemyType == EnemyType::Ugg)
	{
		SpawnWrongWayOrUgg(enemyType);
	}
}

void EnemyManager::DeleteFallenEnemies()
{
	for (size_t i = 0; i < m_SpEnemies.size(); i++)
	{
		auto enemyMovementComp = m_SpEnemies[i]->GetComponent<EnemyMovementComponent>();
		if (enemyMovementComp->GetIsOffScreen() || enemyMovementComp->GetIsDead())
		{
			m_SpEnemies[i]->SetMarkForDelete(true);
			CollisionDetectionManager::GetInstance().DeleteGameObject(m_SpEnemies[i]);

			if (m_SpEnemies[i]->GetName() == "Coily")
			{
				m_CoilySpawned = false;
			}
			else if (m_SpEnemies[i]->GetName() == "WrongWay" || m_SpEnemies[i]->GetName() == "Ugg")
			{
				--m_WrongWayUggAmount;
			}
			else if (m_SpEnemies[i]->GetName() == "Sam" || m_SpEnemies[i]->GetName() == "Slick")
			{
				m_SamOrSlickSpawned = false;
			}

			m_SpEnemies.erase(std::remove(m_SpEnemies.begin(), m_SpEnemies.end(), m_SpEnemies[i]));
		}
	}
}

void EnemyManager::DeleteAllEnemies()
{
	for (auto& spEnemy : m_SpEnemies)
	{
		spEnemy->SetMarkForDelete(true);
		CollisionDetectionManager::GetInstance().DeleteGameObject(spEnemy);
	}
	m_CoilySpawned = false;
	m_SamOrSlickSpawned = false;
	m_WrongWayUggAmount = 0;
	m_SpawnTimerCoily = 0;
	m_SpawnTimerWrongWayUgg = 0;
	m_SpawnTimerSamSlick = 0;

	m_SpEnemies.clear();
}

float EnemyManager::GetEnemySpeedMultiplier() const
{
	return m_SpeedMultiplier;
}

void EnemyManager::SetParametersAccordingToTheLevel(dae::Scene::Level currentLevel)
{
	switch (currentLevel)
	{
	case dae::Scene::Level::FirstLevel:
		m_SpeedMultiplier = 1.0f;
		m_SpawnTimeCoily = 10.0f;
		m_SpawnTimeSamSlick = 15.f;
		m_SpawnTimeWrongWayUgg = 8.f;
		break;
	case dae::Scene::Level::SecondLevel:
		m_SpeedMultiplier = 1.5f;
		m_SpawnTimeCoily = 8.0f;
		m_SpawnTimeSamSlick = 12.f;
		m_SpawnTimeWrongWayUgg = 7.f;
		break;
	case dae::Scene::Level::ThirdLevel:
		m_SpeedMultiplier = 2.0f;
		m_SpawnTimeCoily = 6.f;
		m_SpawnTimeSamSlick = 10.f;
		m_SpawnTimeWrongWayUgg = 6.f;
		break;
	}
}

void EnemyManager::SetWindowSurface(SDL_Surface* windowSurface)
{
	m_pWindowSurface = windowSurface;
}

void EnemyManager::SetLevelObject(const std::shared_ptr<GameObject>& levelObject)
{
	m_LevelObject = levelObject;
}

void EnemyManager::SetPlayers(const std::shared_ptr<GameObject>& pPlayer, const std::shared_ptr<GameObject>& pPlayer2)
{
	m_SpPlayer = pPlayer;
	m_SpPlayer2 = pPlayer2;
}

void EnemyManager::SpawnCoily(dae::Scene::GameMode gameMode)
{
	const int coilyWidth = 16;
	const int coilyHeight = 48;
	const int spawnOffset = 300;

	glm::vec2 coilySpawnPos = glm::vec2(m_LevelObject->GetComponent<PyramidComponent>()->GetSpecificCube(1)->GetPosition().x + coilyWidth, m_LevelObject->GetComponent<PyramidComponent>()->GetSpecificCube(1)->GetPosition().y - coilyHeight);

	auto scene = dae::SceneManager::GetInstance().GetCurrentScene();
	auto coily = std::make_shared<GameObject>("Coily");
	coily->AddComponent(new TransformComponent(glm::vec2(coilySpawnPos.x, coilySpawnPos.y - spawnOffset), glm::vec2(coilyWidth, coilyHeight)));
	coily->AddComponent(new Texture2DComponent("Coily.png", 2, true));
	coily->AddComponent(new AnimationComponent(8));
	if (gameMode == dae::Scene::GameMode::Coop)
	{
		coily->AddComponent(new EnemyMovementComponent(EnemyManager::EnemyType::Coily, m_SpPlayer, m_SpPlayer2));
	}
	else if (gameMode == dae::Scene::GameMode::SinglePlayer)
	{
		coily->AddComponent(new EnemyMovementComponent(EnemyManager::EnemyType::Coily, m_SpPlayer));
	}
	scene->Add(coily);
	CollisionDetectionManager::GetInstance().AddCollisionObject(coily);
	m_SpEnemies.push_back(coily);
}

void EnemyManager::SpawnSamOrSlick(EnemyType enemyType)
{
	const int spawnOffset = 300;

	glm::vec2 samSpawnPos = glm::vec2(m_LevelObject->GetComponent<PyramidComponent>()->GetSpecificCube(7)->GetPosition().x + m_EnemyWidth, m_LevelObject->GetComponent<PyramidComponent>()->GetSpecificCube(7)->GetPosition().y - m_EnemyHeight);
	glm::vec2 slickSpawnPos = glm::vec2(m_LevelObject->GetComponent<PyramidComponent>()->GetSpecificCube(1)->GetPosition().x + m_EnemyWidth, m_LevelObject->GetComponent<PyramidComponent>()->GetSpecificCube(1)->GetPosition().y - m_EnemyHeight);
	auto scene = dae::SceneManager::GetInstance().GetCurrentScene();

	if (enemyType == EnemyType::Sam)
	{
		auto sam = std::make_shared<GameObject>("Sam");
		sam->AddComponent(new TransformComponent(glm::vec2(samSpawnPos.x, samSpawnPos.y - spawnOffset), glm::vec2(m_EnemyWidth, m_EnemyHeight)));
		sam->AddComponent(new Texture2DComponent("Sam.png", 2, true));
		sam->AddComponent(new EnemyMovementComponent(EnemyManager::EnemyType::Sam));
		sam->AddComponent(new AnimationComponent(8));
		scene->Add(sam);
		CollisionDetectionManager::GetInstance().AddCollisionObject(sam);
		m_SpEnemies.push_back(sam);
	}
	else if (enemyType == EnemyType::Slick)
	{
		auto slick = std::make_shared<GameObject>("Slick");
		slick->AddComponent(new TransformComponent(glm::vec2(slickSpawnPos.x, slickSpawnPos.y - spawnOffset), glm::vec2(m_EnemyWidth, m_EnemyHeight)));
		slick->AddComponent(new Texture2DComponent("Slick.png", 2, true));
		slick->AddComponent(new EnemyMovementComponent(EnemyManager::EnemyType::Slick));
		slick->AddComponent(new AnimationComponent(8));
		scene->Add(slick);
		CollisionDetectionManager::GetInstance().AddCollisionObject(slick);
		m_SpEnemies.push_back(slick);
	}
}

void EnemyManager::SpawnWrongWayOrUgg(EnemyType enemyType)
{
	const int spawnOffset = 300;

	glm::vec2 wrongwaySpawnPos = glm::vec2(m_LevelObject->GetComponent<PyramidComponent>()->GetSpecificCube(27)->GetPosition().x - m_EnemyWidth * 1.5f, m_LevelObject->GetComponent<PyramidComponent>()->GetSpecificCube(27)->GetPosition().y + m_EnemyHeight * 3);
	glm::vec2 uggSpawnPos = glm::vec2(m_LevelObject->GetComponent<PyramidComponent>()->GetSpecificCube(6)->GetPosition().x + m_EnemyWidth * 1.5f, m_LevelObject->GetComponent<PyramidComponent>()->GetSpecificCube(6)->GetPosition().y + m_EnemyHeight * 3);
	auto scene = dae::SceneManager::GetInstance().GetCurrentScene();

	if (enemyType == EnemyType::Ugg)
	{
		auto ugg = std::make_shared<GameObject>("Ugg");
		ugg->AddComponent(new TransformComponent(glm::vec2(uggSpawnPos.x + spawnOffset, uggSpawnPos.y - 30), glm::vec2(m_EnemyWidth, m_EnemyHeight)));
		ugg->AddComponent(new Texture2DComponent("Ugg.png", 2, true));
		ugg->AddComponent(new EnemyMovementComponent(EnemyManager::EnemyType::Ugg));
		ugg->AddComponent(new AnimationComponent(8));
		ugg->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::IdleLeftTop);
		scene->Add(ugg);
		CollisionDetectionManager::GetInstance().AddCollisionObject(ugg);
		m_SpEnemies.push_back(ugg);
	}
	else if (enemyType == EnemyType::WrongWay)
	{
		auto wrongWay = std::make_shared<GameObject>("WrongWay");
		wrongWay->AddComponent(new TransformComponent(glm::vec2(wrongwaySpawnPos.x - spawnOffset, wrongwaySpawnPos.y - 30), glm::vec2(m_EnemyWidth, m_EnemyHeight)));
		wrongWay->AddComponent(new Texture2DComponent("WrongWay.png", 2, true));
		wrongWay->AddComponent(new EnemyMovementComponent(EnemyManager::EnemyType::WrongWay));
		wrongWay->AddComponent(new AnimationComponent(8));
		scene->Add(wrongWay);
		CollisionDetectionManager::GetInstance().AddCollisionObject(wrongWay);
		m_SpEnemies.push_back(wrongWay);
	}
}