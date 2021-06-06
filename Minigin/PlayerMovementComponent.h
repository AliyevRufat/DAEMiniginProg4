#pragma once
#include "BaseMovementComponent.h"
#include "Scene.h"
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
	PlayerMovementComponent(const std::string& name, dae::Scene::GameMode gameMode = dae::Scene::GameMode::SinglePlayer, bool isEnemy = false);
	void Update() override;
	void KeyReleased(InputDirection moveDir);
	void Move(InputDirection moveDir);
	//disc movement
	void SetDiscTransform(TransformComponent* discTC);
	void JumpOnDisc();
	void SetPlayerKilled(bool isKilled);
	bool GetIsOnDisc() const { return m_IsOnDisc; };

private:
	//Method
	void OffScreenCheck();
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
	//
	const float m_FreezeTime;
	float m_FreezeTimer;
	bool m_IsKilled;
	//
	dae::Scene::GameMode m_GameMode;
};
