#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include "ComponentIncludes.h"

class CollisionDetectionManager final : public dae::Singleton<CollisionDetectionManager >
{
public:
	void Update();
	void AddCollisionObject(std::shared_ptr<GameObject> gameObject);
private:

	bool IsOverlapping(const SDL_Rect& r1, const SDL_Rect& r2);
	CollisionDetectionManager() = default;
	friend class dae::Singleton<CollisionDetectionManager >;
};
