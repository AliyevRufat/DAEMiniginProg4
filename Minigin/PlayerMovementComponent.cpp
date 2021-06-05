#include "MiniginPCH.h"
#include "PlayerMovementComponent.h"

PlayerMovementComponent::PlayerMovementComponent()
	: m_IsKeyPressed{ false }
{
	m_SourceHeightOffset = 0;
}

void PlayerMovementComponent::Update()
{
	BaseMovementComponent::Update();
}

void PlayerMovementComponent::KeyReleased(InputDirection moveDir)
{
	m_IsKeyPressed[(int)moveDir] = false;
}

void PlayerMovementComponent::Move(InputDirection moveDir)
{
	if (m_IsMoving)
	{
		return;
	}
	m_MoveDirection = moveDir;
	m_IsKeyPressed[(int)moveDir] = true;

	if (m_IsKeyPressed[(int)InputDirection::Up] && m_IsKeyPressed[(int)InputDirection::Left])
	{
		m_Direction = AnimationComponent::AnimationState::JumpLeftTop;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftTop);
		ActivateJump();
		--m_CurrentColumn;
		--m_CurrentRow;
	}
	else if (m_IsKeyPressed[(int)InputDirection::Up] && m_IsKeyPressed[(int)InputDirection::Right])
	{
		m_Direction = AnimationComponent::AnimationState::JumpRightTop;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightTop);
		ActivateJump();
		--m_CurrentRow;
	}
	else if (m_IsKeyPressed[(int)InputDirection::Down] && m_IsKeyPressed[(int)InputDirection::Right])
	{
		m_Direction = AnimationComponent::AnimationState::JumpRightDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightDown);
		ActivateJump();
		++m_CurrentColumn;
		++m_CurrentRow;
	}
	else if (m_IsKeyPressed[(int)InputDirection::Down] && m_IsKeyPressed[(int)InputDirection::Left])
	{
		m_Direction = AnimationComponent::AnimationState::JumpLeftDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftDown);
		ActivateJump();
		++m_CurrentRow;
	}
}