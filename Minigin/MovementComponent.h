#pragma once
#include "ComponentIncludes.h"

//enum class MoveDirection
//{
//	LeftTop,
//	RightTop,
//	RightDown,
//	LeftDown
//};

enum class MoveDirection
{
	Up,
	Down,
	Left,
	Right
};

class MovementComponent final : public BaseComponent
{
public:
	MovementComponent();

	void Update() override;
	void Move(MoveDirection moveDir);
	void KeyReleased(MoveDirection moveDir);
private:
	bool m_Jumped;
	bool m_JumpingUp;
	glm::vec3 m_CurrentPos;
	float m_DesiredPosLeft;
	float m_DesiredPosRight;
	float m_DesiredPosTop;
	float m_DesiredPosBottom;
	//
	glm::vec3 m_JumpDirection;
	MoveDirection m_MoveDirection;
	glm::vec3 m_MoveDistance;
	int m_JumpSpeed;
	const static int m_AmountOfMoves = 4;
	bool m_IsKeyPressed[m_AmountOfMoves];
};
