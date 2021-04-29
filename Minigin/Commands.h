#pragma once
#include <iostream>

#include "HealthComponent.h"
#include "FPSTextComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "MovementComponent.h"

#include "SceneManager.h"
#include "Scene.h"

class Command abstract
{
public:
	Command(int index) :m_ControllerIndex{ index } {};

	virtual ~Command() = default;
	virtual void Execute() const = 0;
	virtual void Undo() = 0;

	[[nodiscard]] bool GetIsPressed() const { return m_IsPressed; };
	void SetIsPressed(bool isPressed) { m_IsPressed = isPressed; };
protected:
	bool m_IsPressed = false;
	int m_ControllerIndex;
};

class FireCommand final : public Command
{
public:
	FireCommand(int index) :Command(index) {};

	void Execute() const override { std::cout << "Fire!" << '\n'; }

	void Undo() override {};
};

class DuckCommand final : public Command
{
public:
	DuckCommand(int index) :Command(index) {};

	void Execute() const override { std::cout << "Duck!" << '\n'; }
	void Undo() override {};
};

class JumpCommand final : public Command
{
public:
	JumpCommand(int index) :Command(index) {};

	void Execute() const override { std::cout << "Jump!" << '\n'; }
	void Undo() override {};
};

class FartCommand final : public Command
{
public:
	FartCommand(int index) :Command(index) {};

	void Execute() const override { std::cout << "Fart..." << '\n'; }
	void Undo() override {};
};

class ExitCommand final : public Command
{
public:
	ExitCommand(int index) :Command(index) {};

	void Execute() const override { std::cout << "Exiting..." << '\n'; }
	void Undo() override {};
};

class DieCommand final : public Command
{
public:
	DieCommand(int index) :Command(index) {};

	void Execute() const override
	{
		std::cout << "Die!" << '\n';
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor.get()->GetComponent<HealthComponent>()->Die();
	}

	void Undo() override {};
};

class IncreasePointsCommand final : public Command
{
public:
	IncreasePointsCommand(int index) :Command(index) {};

	void Execute() const override
	{
		std::cout << "PointIncrease" << std::endl;
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor.get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::ColorChanged);
	}

	void Undo() override {};
};
//-------------------------------------------------------------------------------------------------------IDLE MOVEMENT-------------------------------------------------------
class MoveLeftTop final : public Command
{
public:
	MoveLeftTop(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState::IdleLeftTop);
		//pPlayerActor.get()->GetComponent<TransformComponent>()->MoveLeftTop ? ? ? ;
	}

	void Undo() override {};
};

class MoveRightTop final : public Command
{
public:
	MoveRightTop(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState::IdleRightTop);
		//pPlayerActor.get()->GetComponent<TransformComponent>()->MoveLeftTop ? ? ? ;
	}

	void Undo() override {};
};

class MoveRightDown final : public Command
{
public:
	MoveRightDown(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState::IdleRightDown);
		//pPlayerActor.get()->GetComponent<TransformComponent>()->MoveLeftTop ? ? ? ;
	}

	void Undo() override {};
};

class MoveLeftDown final : public Command
{
public:
	MoveLeftDown(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState::IdleLeftDown);
		//pPlayerActor.get()->GetComponent<TransformComponent>()->MoveLeftTop ? ? ? ;
	}

	void Undo() override {};
};
//-------------------------------------------------------------------------------------------------------JUMP MOVEMENT-------------------------------------------------------
class JumpLeftTop final : public Command
{
public:
	JumpLeftTop(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState::JumpLeftTop);
		pPlayerActor->GetComponent<MovementComponent>()->Move(MoveDirection::LeftTop);
	}

	void Undo() override {};
};

class JumpRightTop final : public Command
{
public:
	JumpRightTop(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState::JumpRightTop);
		pPlayerActor->GetComponent<MovementComponent>()->Move(MoveDirection::RightTop);
	}

	void Undo() override {};
};

class JumpRightDown final : public Command
{
public:
	JumpRightDown(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState::JumpRightDown);
		pPlayerActor->GetComponent<MovementComponent>()->Move(MoveDirection::RightDown);
	}

	void Undo() override {};
};

class JumpLeftDown final : public Command
{
public:
	JumpLeftDown(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState::JumpLeftDown);
		pPlayerActor->GetComponent<MovementComponent>()->Move(MoveDirection::LeftDown);
	}

	void Undo() override {};
};