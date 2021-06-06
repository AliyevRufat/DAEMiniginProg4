#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include "ComponentIncludes.h"

class TransformComponent;

class CollisionDetectionManager final : public dae::Singleton<CollisionDetectionManager >
{
public:
	void Update();
	void AddCollisionObject(std::shared_ptr<GameObject> gameObject);
	void DeleteGameObject(std::shared_ptr<GameObject> gameObject);

private:
	//Methods
	bool IsOverlapping(const SDL_Rect& r1, const SDL_Rect& r2);
	CollisionDetectionManager() = default;
	friend class dae::Singleton<CollisionDetectionManager>;
	//Datamembers
	std::vector<TransformComponent*> m_pOtherEntityTransforms;
	TransformComponent* m_pQbertTransform;
	//
	std::vector<std::shared_ptr<GameObject>> m_pOtherEntities;
	std::shared_ptr<GameObject> m_pQbert;
};
