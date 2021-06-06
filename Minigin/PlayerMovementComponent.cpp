#include "MiniginPCH.h"
#include "PlayerMovementComponent.h"
#include "PyramidComponent.h"
#include "SceneManager.h"
#include "Transform.h"

PlayerMovementComponent::PlayerMovementComponent(bool isEnemy)
	: m_IsKeyPressed{ false }
	, m_IsEnemy{ isEnemy }
{
	if (m_IsEnemy)
	{
		m_Speed *= 0.6f;
	}
	m_SourceHeightOffset = 0;
}

void PlayerMovementComponent::Update()
{
	if (m_FallingToDeath)
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
			dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>()->TeleportPlayersToSpawnPos();
			m_CurrentCubeIndex = 0;
			m_IsOnDisc = false;
			m_JumpingOnDisc = false;
			m_FallingToDeath = false;
		}
		else
		{
			float elapsedTime = Time::GetInstance().GetDeltaTime();

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
				m_FallingToDeath = false;
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
		m_FallingToDeath = true;
	}
	else if (!fallOfMap && CurrentMap->GetSpecificCube(m_CurrentCubeIndex)->GetHasDiscNextToIt())
	{
		m_JumpingOnDisc = true;
	}
}