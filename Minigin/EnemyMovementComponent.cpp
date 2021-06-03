#include "MiniginPCH.h"
#include "EnemyMovementComponent.h"
#include "TransformComponent.h"

EnemyMovementComponent::EnemyMovementComponent(const std::shared_ptr<GameObject>& pPlayer, EnemyType enemyType)
	:m_pPlayer{ pPlayer }
	, m_CurrentJumpTime{ 0.0f }
	, m_MaxJumpTime{ 1.5f }
	, m_EnemyType{ enemyType }
{
	switch (enemyType)
	{
	case EnemyMovementComponent::EnemyType::Coily:
		m_SourceHeightOffset = -8;
		break;
	case EnemyMovementComponent::EnemyType::Sam:
		m_SourceHeightOffset = 0;
		break;
	case EnemyMovementComponent::EnemyType::Slick:
		m_SourceHeightOffset = 0;
		break;
	case EnemyMovementComponent::EnemyType::Ugg:
		m_SourceHeightOffset = 0;
		break;
	case EnemyMovementComponent::EnemyType::WrongWay:
		m_SourceHeightOffset = 0;
		break;
	}
}

void EnemyMovementComponent::Update()
{
	BaseMovementComponent::Update();

	m_CurrentJumpTime += Time::GetInstance().GetDeltaTime();

	if (m_CurrentJumpTime >= m_MaxJumpTime)
	{
		switch (m_EnemyType)
		{
		case EnemyMovementComponent::EnemyType::Coily:
			FollowPlayer();
			break;
		case EnemyMovementComponent::EnemyType::Sam:
			Descend();
			break;
		case EnemyMovementComponent::EnemyType::Slick:
			Descend();
			break;
		case EnemyMovementComponent::EnemyType::Ugg:
			break;
		case EnemyMovementComponent::EnemyType::WrongWay:
			break;
		}

		m_CurrentJumpTime -= m_CurrentJumpTime;
	}
}

void EnemyMovementComponent::FollowPlayer()
{
	if (m_IsMoving)
	{
		return;
	}

	const int yOffset = 30;
	auto playerPos = m_pPlayer->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	auto pos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();

	if (playerPos.x <= pos.x && playerPos.y - yOffset <= pos.y) // left top
	{
		m_Direction = AnimationComponent::AnimationState::JumpLeftTop;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftTop);
		ActivateJump();
	}
	else if (playerPos.x >= pos.x && playerPos.y - yOffset <= pos.y) // right top
	{
		m_Direction = AnimationComponent::AnimationState::JumpRightTop;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightTop);
		ActivateJump();
	}
	else if (playerPos.x <= pos.x && playerPos.y >= pos.y) // left bottom
	{
		m_Direction = AnimationComponent::AnimationState::JumpLeftDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftDown);
		ActivateJump();
	}
	else if (playerPos.x >= pos.x && playerPos.y >= pos.y) // right down
	{
		m_Direction = AnimationComponent::AnimationState::JumpRightDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightDown);
		ActivateJump();
	}
}

void EnemyMovementComponent::Descend()
{
	if (m_IsMoving)
	{
		return;
	}

	int randNr = rand() % 2;

	if (randNr == 0)
	{
		m_Direction = AnimationComponent::AnimationState::JumpLeftDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftDown);
		ActivateJump();
	}
	else
	{
		m_Direction = AnimationComponent::AnimationState::JumpRightDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightDown);
		ActivateJump();
	}
}

void EnemyMovementComponent::SidewaysMovement()
{
}