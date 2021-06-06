#include "MiniginPCH.h"
#include "CollisionDetectionManager.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "PyramidComponent.h"
#include "HealthComponent.h"
#include "PlayerMovementComponent.h"

void CollisionDetectionManager::Update()
{
	for (size_t i = 0; i < m_pOtherEntities.size(); i++)
	{
		if (IsOverlapping(m_pQbertTransform->GetRect(), m_pOtherEntityTransforms[i]->GetRect()))
		{
			if (m_pOtherEntities[i]->GetName() == "Disc")
			{
				m_pQbert->GetComponent<PlayerMovementComponent>()->SetDiscTransform(m_pOtherEntities[i]->GetComponent<TransformComponent>());
				dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>()->GetDisc(m_pOtherEntities[i])->SetIsMovingToTop(true);
			}
			else if (m_pOtherEntities[i]->GetName() == "Ugg" || m_pOtherEntities[i]->GetName() == "WrongWay" || m_pOtherEntities[i]->GetName() == "Coily")
			{
				m_pQbert->GetComponent<HealthComponent>()->Die();
			}
			else if (m_pOtherEntities[i]->GetName() == "Sam" || m_pOtherEntities[i]->GetName() == "Slick")
			{
				m_pOtherEntities[i]->GetComponent<HealthComponent>()->Die();
				//add score
			}
		}
	}
}

void CollisionDetectionManager::AddCollisionObject(std::shared_ptr<GameObject> gameObject)
{
	auto transform = gameObject->GetComponent<TransformComponent>();

	if (gameObject->GetName() == "Q*Bert")
	{
		m_pQbertTransform = transform;
		m_pQbert = gameObject;
	}
	else
	{
		m_pOtherEntityTransforms.push_back(transform);
		m_pOtherEntities.push_back(gameObject);
	}
}

bool CollisionDetectionManager::IsOverlapping(const SDL_Rect& r1, const SDL_Rect& r2)
{
	// If one rectangle is on left side of the other
	if ((r1.x + r1.w) < r2.x || (r2.x + r2.w) < r1.x)
	{
		return false;
	}

	// If one rectangle is under the other
	if (r1.y > (r2.y + r2.h) || r2.y > (r1.y + r1.h))
	{
		return false;
	}

	return true;
}

void CollisionDetectionManager::DeleteGameObject(std::shared_ptr<GameObject> gameObject)
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