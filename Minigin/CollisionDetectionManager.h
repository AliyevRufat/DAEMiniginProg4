#pragma once
#include "../AliEngine/Singleton.h"
#include "../AliEngine/GameObject.h"
#include "ComponentIncludes.h"

class TransformComponent;

class CollisionDetectionManager final : public dae::Singleton<CollisionDetectionManager>
{
public:
	void Update();
	void AddCollisionObject(const std::shared_ptr<GameObject>& gameObject);
	void DeleteGameObject(const std::shared_ptr<GameObject>& gameObject);
	void ClearCollisions();
private:
	//Methods
	bool IsOverlapping(const SDL_Rect& r1, const SDL_Rect& r2);
	CollisionDetectionManager() = default;
	friend class dae::Singleton<CollisionDetectionManager>;
	//Datamembers
	std::vector<TransformComponent*> m_pOtherEntityTransforms;
	TransformComponent* m_pQbertTransform = nullptr;
	TransformComponent* m_pQbertTransform2 = nullptr;
	//
	std::vector<std::shared_ptr<GameObject>> m_pOtherEntities;
	std::shared_ptr<GameObject> m_pQbert;
	std::shared_ptr<GameObject> m_pQbert2;
};
