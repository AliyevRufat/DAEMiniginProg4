#pragma once
#include "BaseMovementComponent.h"

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
	PlayerMovementComponent();
	void Update() override;
	void KeyReleased(InputDirection moveDir);
	void Move(InputDirection moveDir);

private:
	const static int m_AmountOfMoves = 4;
	bool m_IsKeyPressed[m_AmountOfMoves];
	InputDirection m_MoveDirection;
};
