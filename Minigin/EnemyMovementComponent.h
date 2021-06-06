#pragma once
#include "../AliEngine/GameObject.h"
#include "EnemyManager.h"
#include "BaseMovementComponent.h"

class EnemyMovementComponent final : public BaseMovementComponent
{
public:

	EnemyMovementComponent(EnemyManager::EnemyType enemyType, const std::shared_ptr<GameObject>& pPlayer = nullptr, const std::shared_ptr<GameObject>& pPlayer2 = nullptr);
	void Update();
	bool GetIsDead() const { return m_IsDead; };
	void SetIsDead(bool isDead) { m_IsDead = isDead; };
private:
	//Methods
	void FollowPlayer();
	void Descend();
	void SidewaysMovement();
	void SidewaysJump();
	void SidewaysFall();
	bool IsOnSpawnCube();

	//Datamembers
	EnemyManager::EnemyType m_EnemyType;
	std::shared_ptr<GameObject> m_pPlayer;
	std::shared_ptr<GameObject> m_pPlayer2;
	float m_CurrentJumpTime;
	float m_MaxJumpTime;
	float m_SpeedMultiplier;
	bool m_IsDead;
	bool m_IsOnSpawnCube;
};
