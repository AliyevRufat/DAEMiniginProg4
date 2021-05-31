#pragma once
#include "ComponentIncludes.h"
#include "AnimationComponent.h"

enum class InputDirection
{
	Up,
	Down,
	Left,
	Right
};

class MovementComponent final : public BaseComponent
{
public:
	MovementComponent(const std::shared_ptr<GameObject>& pPlayer = nullptr);

	void Update() override;

	void Move(InputDirection moveDir);
	void KeyReleased(InputDirection moveDir);

private:
	//Methods
	bool GetIsFallingToDeathBehindMap() const { return m_FallingToDeath && !m_FirstHalfOfTheJump && (m_Direction == AnimationComponent::AnimationState::JumpLeftTop || m_Direction == AnimationComponent::AnimationState::JumpRightTop); };
	void SetCurrentCubeIndex(int index) { m_CurrentCubeIndex = index; };
	void ActivateJump();
	void FollowPlayer();
	void FallToDeath();
	void Jump();
	//
	const static int m_AmountOfMoves = 4;
	bool m_IsKeyPressed[m_AmountOfMoves];
	InputDirection m_MoveDirection;

	bool m_IsMoving;
	bool m_FallingToDeath;
	float m_Speed;
	glm::vec2 m_MoveDistance;
	bool m_FirstHalfOfTheJump;
	AnimationComponent::AnimationState m_Direction;
	glm::vec2 m_JumpStartPos;
	int m_CurrentCubeIndex;
	//coily
	std::shared_ptr<GameObject> m_pPlayer;
	float m_CurrentCoilyJumpTime;
	const float m_MaxCoilyJumpTime;
};
