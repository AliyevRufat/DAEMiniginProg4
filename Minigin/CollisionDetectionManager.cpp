#include "MiniginPCH.h"
#include "CollisionDetectionManager.h"
#include "../AliEngine/TransformComponent.h"
#include "../AliEngine/SceneManager.h"
#include "PyramidComponent.h"
#include "HealthComponent.h"
#include "../AliEngine/Scene.h"
#include "PlayerMovementComponent.h"
#include "EnemyManager.h"
#include "ScoreComponent.h"
#include "../AliEngine/Locator.h"
#include "EnemyMovementComponent.h"

void CollisionDetectionManager::Update()
{
	if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() == dae::Scene::GameMode::None)
	{
		return;
	}
	if (m_pQbert->GetComponent<PlayerMovementComponent>()->GetIsOnDisc())
	{
		return;
	}
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		if (IsOverlapping(m_pQbertTransform->GetRect(), m_pOtherEntityTransforms[i]->GetRect()))
		{
			if (m_pOtherEntities[i]->GetName() == "Disc")
			{
				Locator::GetAudio().QueueSound(AudioService::SoundIds::LiftEffect, true, 50);
				m_pQbert->GetComponent<PlayerMovementComponent>()->SetDiscTransform(m_pOtherEntities[i]->GetComponent<TransformComponent>());
				dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>()->GetDisc(m_pOtherEntities[i])->SetIsMovingToTop(true);
			}
			else if (m_pOtherEntities[i]->GetName() == "Ugg" || m_pOtherEntities[i]->GetName() == "WrongWay" || m_pOtherEntities[i]->GetName() == "Coily")
			{
				m_pQbert->GetComponent<PlayerMovementComponent>()->SetPlayerKilled(true);
				if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() == dae::Scene::GameMode::Versus)
				{
					dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>()->TeleportPlayersToCorrectPos(dae::Scene::GameMode::Versus);
				}
				else
				{
					EnemyManager::GetInstance().DeleteAllEnemies();
				}
				break;
			}
			else if (m_pOtherEntities[i]->GetName() == "Sam" || m_pOtherEntities[i]->GetName() == "Slick")
			{
				dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0).get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::CatchSlickOrSam);
				m_pOtherEntities[i]->GetComponent<EnemyMovementComponent>()->SetIsDead(true);
			}
		}
		if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() == dae::Scene::GameMode::Coop)
		{
			if (m_pQbert2->GetComponent<PlayerMovementComponent>()->GetIsOnDisc())
			{
				return;
			}
			if (IsOverlapping(m_pQbertTransform2->GetRect(), m_pOtherEntityTransforms[i]->GetRect()))
			{
				if (m_pOtherEntities[i]->GetName() == "Disc")
				{
					Locator::GetAudio().QueueSound(AudioService::SoundIds::LiftEffect, true, 50);
					m_pQbert2->GetComponent<PlayerMovementComponent>()->SetDiscTransform(m_pOtherEntities[i]->GetComponent<TransformComponent>());
					dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>()->GetDisc(m_pOtherEntities[i])->SetIsMovingToTop(true);
				}
				else if (m_pOtherEntities[i]->GetName() == "Ugg" || m_pOtherEntities[i]->GetName() == "WrongWay" || m_pOtherEntities[i]->GetName() == "Coily")
				{
					m_pQbert2->GetComponent<PlayerMovementComponent>()->SetPlayerKilled(true);
					EnemyManager::GetInstance().DeleteAllEnemies();
					break;
				}
				else if (m_pOtherEntities[i]->GetName() == "Sam" || m_pOtherEntities[i]->GetName() == "Slick")
				{
					dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1).get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::CatchSlickOrSam);
					m_pOtherEntities[i]->GetComponent<EnemyMovementComponent>()->SetIsDead(true);
				}
			}
		}
	}
}

void CollisionDetectionManager::AddCollisionObject(const std::shared_ptr<GameObject>& gameObject)
{
	auto transform = gameObject->GetComponent<TransformComponent>();

	if (gameObject->GetName() == "Q*Bert")
	{
		m_pQbertTransform = transform;
		m_pQbert = gameObject;
	}
	else if (gameObject->GetName() == "Q*Bert2")
	{
		m_pQbertTransform2 = transform;
		m_pQbert2 = gameObject;
	}
	else
	{
		m_pOtherEntityTransforms.push_back(transform);
		m_pOtherEntities.push_back(gameObject);
	}
}

bool CollisionDetectionManager::IsOverlapping(const SDL_Rect& r1, const SDL_Rect& r2)
{
	if ((r1.x + r1.w) < r2.x || (r2.x + r2.w) < r1.x)
	{
		return false;
	}

	if (r1.y > (r2.y + r2.h) || r2.y > (r1.y + r1.h))
	{
		return false;
	}

	return true;
}

void CollisionDetectionManager::DeleteGameObject(const std::shared_ptr<GameObject>& gameObject)
{
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		if (m_pOtherEntities[i] == gameObject)
		{
			m_pOtherEntities.erase(m_pOtherEntities.begin() + i);
			m_pOtherEntityTransforms.erase(m_pOtherEntityTransforms.begin() + i);
		}
	}
}

void CollisionDetectionManager::ClearCollisions()
{
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		m_pOtherEntities.erase(m_pOtherEntities.begin() + i);
		m_pOtherEntityTransforms.erase(m_pOtherEntityTransforms.begin() + i);
		--i;
	}
	m_pQbertTransform = nullptr;
	m_pQbertTransform2 = nullptr;
	for (size_t i = 0; i < m_pOtherEntityTransforms.size(); i++)
	{
		m_pOtherEntityTransforms[i] = nullptr;
	}
}