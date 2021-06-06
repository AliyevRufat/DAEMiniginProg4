#include "MiniginPCH.h"
#include "PlayerMovementComponent.h"
#include "PyramidComponent.h"
#include "../AliEngine/SceneManager.h"
#include "EnemyManager.h"
#include "../AliEngine/Transform.h"
#include "HealthComponent.h"

PlayerMovementComponent::PlayerMovementComponent(const std::string& name, dae::Scene::GameMode gameMode, bool isEnemy)
	: m_IsKeyPressed{ false }
	, m_IsEnemy{ isEnemy }
	, m_FreezeTime{ 2.0f }
	, m_FreezeTimer{ 0.0f }
	, m_IsKilled{ false }
	, m_JumpedRightDown{ false }
	, m_JumpDown{ false }
	, m_GameMode{ gameMode }
{
	if (m_IsEnemy)
	{
		m_Speed *= 0.8f;
		m_JumpDown = true;
	}
	if (gameMode == dae::Scene::GameMode::Coop)
	{
		if (name == "Q*Bert")
		{
			m_CurrentColumn = 0;
			m_CurrentRow = 6;
			m_CurrentCubeIndex = 27;
		}
		else if (name == "Q*Bert2")
		{
			m_CurrentColumn = 6;
			m_CurrentRow = 6;
			m_CurrentCubeIndex = 6;
		}
	}
	if (gameMode == dae::Scene::GameMode::Versus)
	{
		if (name == "Q*Bert")
		{
			m_CurrentColumn = 0;
			m_CurrentRow = 6;
			m_CurrentCubeIndex = 27;
		}
	}
	m_SourceHeightOffset = 0;
}

void PlayerMovementComponent::Update()
{
	if (m_JumpDown)
	{
		GoingDownThePyramid();
	}

	if (m_IsKilled)
	{
		m_FreezeTimer += EngineTime::GetInstance().GetDeltaTime();
		if (m_FreezeTimer >= m_FreezeTime)
		{
			m_FreezeTimer -= m_FreezeTimer;
			m_IsKilled = false;
		}
		return;
	}

	OffScreenCheck();

	if (m_IsFalling)
	{
		FallToDeath();
	}
	else if (m_JumpingOnDisc)
	{
		JumpOnDisc();
	}
	else if (m_IsMoving)
	{
		Jump();
	}
}

void PlayerMovementComponent::SetPlayerKilled(bool isKilled)
{
	m_IsKilled = isKilled;
	m_pGameObject->GetComponent<HealthComponent>()->Die();
}

void PlayerMovementComponent::OffScreenCheck()
{
	if (m_IsOffScreen)
	{
		dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>()->TeleportPlayersToCorrectPos(m_GameMode);
		if (!m_IsEnemy)
		{
			m_pGameObject->GetComponent<HealthComponent>()->Die();
		}
		EnemyManager::GetInstance().DeleteAllEnemies();
		m_IsOffScreen = false;
	}
}

void PlayerMovementComponent::KeyReleased(InputDirection moveDir)
{
	m_IsKeyPressed[(int)moveDir] = false;
}

void PlayerMovementComponent::Move(InputDirection moveDir)
{
	if (m_IsMoving || !m_pGameObject->GetIsActive())
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

void PlayerMovementComponent::SetDiscTransform(TransformComponent* discTC)
{
	m_pDiscTransform = discTC;
}

void PlayerMovementComponent::JumpOnDisc()
{
	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();

	if (!m_pDiscTransform)
	{
		if (m_IsOnDisc)
		{
			dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>()->TeleportPlayersToCorrectPos(m_GameMode);
			m_CurrentCubeIndex = 0;
			m_IsOnDisc = false;
			m_JumpingOnDisc = false;
			m_IsFalling = false;
		}
		else
		{
			float elapsedTime = EngineTime::GetInstance().GetDeltaTime();

			glm::vec2 pos = transform->GetTransform().GetPosition();

			const float moveDistRatio = (m_MoveDistance.y / m_MoveDistance.x);

			float jumpHeight = m_MoveDistance.y / 2.0f;

			const glm::vec2 speed = { m_Speed,m_Speed * moveDistRatio * (m_MoveDistance.y / jumpHeight) };

			if (m_Direction == AnimationComponent::AnimationState::JumpRightDown || m_Direction == AnimationComponent::AnimationState::JumpRightTop)
				pos.x += elapsedTime * speed.x;
			else
				pos.x -= elapsedTime * speed.x;

			jumpHeight = m_MoveDistance.y * 1.5f;

			if (m_FirstHalfOfTheJump)
			{
				pos.y -= elapsedTime * speed.y;

				if (abs(pos.y - m_JumpStartPos.y) > jumpHeight)
					m_FirstHalfOfTheJump = false;
			}
			else pos.y += elapsedTime * speed.y;

			if (pos.y > 720)
			{
				m_IsMoving = false;
				m_IsFalling = false;
			}
			else transform->SetPosition(glm::vec2(pos.x, pos.y));
		}
	}
	else
	{
		m_IsOnDisc = true;
		glm::vec2 newPos = m_pDiscTransform->GetTransform().GetPosition();
		newPos.x += dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale() * 1.f;
		newPos.y -= dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale() * 13.f;
		transform->SetPosition(newPos);
	}
}

void PlayerMovementComponent::ActivateJump(bool isSideWaysJump)
{
	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();

	m_JumpStartPos = transform->GetTransform().GetPosition();
	m_IsMoving = true;
	m_FirstHalfOfTheJump = true;

	const auto& CurrentMap = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>();

	bool fallOfMap = CurrentMap->GetNextCubeIndex(m_CurrentCubeIndex, m_Direction, isSideWaysJump, m_CurrentColumn, m_CurrentRow);

	if (!fallOfMap && !CurrentMap->GetSpecificCube(m_CurrentCubeIndex)->GetHasDiscNextToIt())
	{
		m_IsFalling = true;
	}
	else if (!fallOfMap && CurrentMap->GetSpecificCube(m_CurrentCubeIndex)->GetHasDiscNextToIt())
	{
		m_JumpingOnDisc = true;
	}
}

void PlayerMovementComponent::GoingDownThePyramid()
{
	if (m_IsMoving)
	{
		return;
	}

	if (m_CurrentRow == 5)
	{
		m_JumpDown = false;
	}

	if (m_JumpedRightDown)
	{
		m_Direction = AnimationComponent::AnimationState::JumpLeftDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftDown);
		ActivateJump();
		++m_CurrentRow;
	}
	else
	{
		m_Direction = AnimationComponent::AnimationState::JumpRightDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightDown);
		ActivateJump();
		++m_CurrentColumn;
		++m_CurrentRow;
	}
	m_JumpedRightDown = !m_JumpedRightDown;
}