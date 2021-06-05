#pragma once
#include "ComponentIncludes.h"
#include "AnimationComponent.h"

class BaseMovementComponent : public BaseComponent
{
public:
	explicit BaseMovementComponent();
	virtual void Update() = 0;

protected:
	//Methods
	bool GetIsFallingToDeathBehindMap() const { return m_FallingToDeath && !m_FirstHalfOfTheJump && (m_Direction == AnimationComponent::AnimationState::JumpLeftTop || m_Direction == AnimationComponent::AnimationState::JumpRightTop); };
	void SetCurrentCubeIndex(int index) { m_CurrentCubeIndex = index; };
	void ActivateJump(bool isSideWaysJump = false);
	void FallToDeath();
	void Jump();
	//
	bool m_IsMoving;
	bool m_FallingToDeath;
	float m_Speed;
	glm::vec2 m_MoveDistance;
	bool m_FirstHalfOfTheJump;
	AnimationComponent::AnimationState m_Direction;
	glm::vec2 m_JumpStartPos;
	int m_CurrentCubeIndex;
	int m_SourceHeightOffset;
	int m_CurrentColumn;
	int m_CurrentRow;
};
