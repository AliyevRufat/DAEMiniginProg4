#pragma once
#include "BaseMovementComponent.h"
#include "TransformComponent.h"

enum class InputDirection
{
	Up,
	Down,
	Left,
	Right
};

class PlayerMovementComponent final : public BaseMovementComponent
{
public:
	PlayerMovementComponent(bool isEnemy = false);
	void Update() override;
	void KeyReleased(InputDirection moveDir);
	void Move(InputDirection moveDir);
	//disc movement
	void SetDiscTransform(TransformComponent* discTC);
	void JumpOnDisc();
private:
	//Method
	void ActivateJump(bool isSideWaysJump = false) override;
	//Datamember
	const static int m_AmountOfMoves = 4;
	bool m_IsKeyPressed[m_AmountOfMoves];
	InputDirection m_MoveDirection;
	TransformComponent* m_pDiscTransform;
	//
	bool m_JumpingOnDisc;
	bool m_IsOnDisc;
	bool m_IsEnemy;
};
