#include "MiniginPCH.h"
#include "CollisionDetectionManager.h"

void CollisionDetectionManager::Update()
{
}

void CollisionDetectionManager::AddCollisionObject(std::shared_ptr<GameObject> gameObject)
{
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