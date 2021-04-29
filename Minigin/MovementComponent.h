#pragma once
#include "ComponentIncludes.h"

enum class MoveDirection
{
	LeftTop,
	RightTop,
	RightDown,
	LeftDown
};

class MovementComponent final : public BaseComponent
{
public:
	MovementComponent();

	void Update() override;
	void Move(MoveDirection moveDir);

private:
	bool m_Jumped;
	bool m_JumpingUp;
	MoveDirection m_MoveDirection;
	glm::vec3 m_CurrentPos;
	float m_DesiredPosLeft;
	float m_DesiredPosRight;
	float m_DesiredPosTop;
	float m_DesiredPosBottom;
};
