#include "MiniginPCH.h"
#include "CollisionDetectionManager.h"
#include "TransformComponent.h"

void CollisionDetectionManager::Update()
{
	for (auto& transform : m_pTransform)
	{
		if (IsOverlapping(m_pQbertTransform->GetRect(), transform->GetRect()))
		{
			//player death
		}
	}
}

void CollisionDetectionManager::AddCollisionObject(std::shared_ptr<GameObject> gameObject)
{
	auto transform = gameObject->GetComponent<TransformComponent>();

	if (gameObject->GetName() == "Q*Bert")
	{
		m_pQbertTransform = transform;
	}
	else
	{
		m_pTransform.push_back(transform);
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