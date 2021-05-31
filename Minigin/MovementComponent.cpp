#include "MiniginPCH.h"
#include "MovementComponent.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "SceneManager.h"
#include "PyramidComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Time.h"
#include "Texture2DComponent.h"

MovementComponent::MovementComponent(const std::shared_ptr<GameObject>& pPlayer)
	: m_Speed{ 100 }
	, m_IsMoving{ false }
	, m_CurrentCubeIndex{ 0 }
	, m_FallingToDeath{ false }
	, m_Direction{ AnimationComponent::AnimationState::IdleLeftDown }
	, m_IsKeyPressed{ false }
	, m_pPlayer{ pPlayer }
	, m_CurrentCoilyJumpTime{ 0.0f }
	, m_MaxCoilyJumpTime{ 1.5f }
{
	const glm::vec2& cubeOffset = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>()->GetCubeOffset();
	m_MoveDistance = cubeOffset;
}

void MovementComponent::Update()
{
	if (m_FallingToDeath)
	{
		FallToDeath();
	}
	else if (m_IsMoving)
	{
		Jump();
	}

	if (m_pGameObject->GetName() == "Coily")
	{
		m_CurrentCoilyJumpTime += Time::GetInstance().GetDeltaTime();

		if (m_CurrentCoilyJumpTime >= m_MaxCoilyJumpTime)
		{
			FollowPlayer();
			m_CurrentCoilyJumpTime -= m_CurrentCoilyJumpTime;
		}
	}
}

void MovementComponent::FollowPlayer()
{
	if (m_IsMoving)
	{
		return;
	}
	auto playerPos = m_pPlayer->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	auto pos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();

	if (playerPos.x < pos.x && playerPos.y < pos.y) // left top
	{
		m_Direction = AnimationComponent::AnimationState::JumpLeftTop;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftTop);
		ActivateJump();
	}
	else if (playerPos.x < pos.x && playerPos.y > pos.y) // left bottom
	{
		m_Direction = AnimationComponent::AnimationState::JumpLeftDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftDown);
		ActivateJump();
	}
	else if (playerPos.x > pos.x && playerPos.y < pos.y) // right top
	{
		m_Direction = AnimationComponent::AnimationState::JumpRightTop;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightTop);
		ActivateJump();
	}
	else if (playerPos.x > pos.x && playerPos.y > pos.y) // right down
	{
		m_Direction = AnimationComponent::AnimationState::JumpRightDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightDown);
		ActivateJump();
	}
}

void MovementComponent::ActivateJump()
{
	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();

	m_JumpStartPos = transform->GetTransform().GetPosition();
	m_IsMoving = true;
	m_FirstHalfOfTheJump = true;

	const auto& CurrentMap = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>();

	if (!CurrentMap->GetNextCubeIndex(m_CurrentCubeIndex, m_Direction))
	{
		// Player jumped off the map
		m_FallingToDeath = true;
	}
}

void MovementComponent::Jump()
{
	float deltaTime = Time::GetInstance().GetDeltaTime();
	//
	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();
	glm::vec2 pos = transform->GetTransform().GetPosition();

	const float moveDistanceRatio = (m_MoveDistance.y / m_MoveDistance.x);
	float jumpHeight = m_MoveDistance.y / 2.0f;
	const glm::vec2 speed = { m_Speed,m_Speed * moveDistanceRatio * (m_MoveDistance.y / jumpHeight) };

	if (m_Direction == AnimationComponent::AnimationState::JumpRightDown || m_Direction == AnimationComponent::AnimationState::JumpRightTop)
		pos.x += deltaTime * speed.x;
	else
		pos.x -= deltaTime * speed.x;

	if ((int)m_Direction >= (int)AnimationComponent::AnimationState::IdleRightDown)
		jumpHeight = m_MoveDistance.y / 2.0f;
	else
		jumpHeight = m_MoveDistance.y * 1.5f;

	if (m_FirstHalfOfTheJump)
	{
		pos.y -= deltaTime * speed.y;

		if (abs(pos.y - m_JumpStartPos.y) > jumpHeight)
			m_FirstHalfOfTheJump = false;
	}
	else
	{
		pos.y += deltaTime * speed.y;
	}

	if (abs(pos.x - m_JumpStartPos.x) > m_MoveDistance.x)
	{
		// Done with jumping

		// Direction - 1 = not jumping version of sprite
		int NonJumpingSprite = (int)m_Direction - 1;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState(NonJumpingSprite));
		const auto& CurrentMap = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>();

		auto cube = CurrentMap->GetCube(m_CurrentCubeIndex);
		cube->Color();
		//offset fix
		auto cubePos = cube->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		auto srcRect = m_pGameObject->GetComponent<Texture2DComponent>()->GetSrcRect();

		if (m_pGameObject->GetName() == "Coily")
		{
			const int offset = 16;
			srcRect.h += offset;
		}
		m_pGameObject->GetComponent<TransformComponent>()->SetPosition(glm::vec2(cubePos.x + srcRect.w, cubePos.y - srcRect.h));

		m_IsMoving = false;
	}
	else
	{
		transform->SetPosition(pos);
	}
}

void MovementComponent::FallToDeath()
{
	float deltaTime = Time::GetInstance().GetDeltaTime();
	//

	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();
	glm::vec2 pos = transform->GetTransform().GetPosition();

	const float moveDistanceRatio = (m_MoveDistance.y / m_MoveDistance.x);
	float jumpHeight = m_MoveDistance.y / 2.0f;
	const glm::vec2 speed = { m_Speed,m_Speed * moveDistanceRatio * (m_MoveDistance.y / jumpHeight) };

	if (m_Direction == AnimationComponent::AnimationState::JumpRightDown || m_Direction == AnimationComponent::AnimationState::JumpRightTop)
		pos.x += deltaTime * speed.x;
	else
		pos.x -= deltaTime * speed.x;

	if ((int)m_Direction >= (int)AnimationComponent::AnimationState::IdleRightDown)
		jumpHeight = m_MoveDistance.y / 2.0f;
	else
		jumpHeight = m_MoveDistance.y * 1.5f;

	if (m_FirstHalfOfTheJump)
	{
		pos.y -= deltaTime * speed.y;

		if (abs(pos.y - m_JumpStartPos.y) > jumpHeight)
			m_FirstHalfOfTheJump = false;
	}
	else
	{
		pos.y += deltaTime * speed.y;
	}

	if (pos.y > 1540) //width is 1540
	{
		int NonJumpingSprite = (int)m_Direction - 1;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState(NonJumpingSprite));
		m_IsMoving = false;
		m_FallingToDeath = false;
	}
	else
	{
		transform->SetPosition(pos);
	}
}

void MovementComponent::Move(InputDirection moveDir)
{
	if (m_IsMoving)
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
	}
	else if (m_IsKeyPressed[(int)InputDirection::Up] && m_IsKeyPressed[(int)InputDirection::Right])
	{
		m_Direction = AnimationComponent::AnimationState::JumpRightTop;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightTop);
		ActivateJump();
	}
	else if (m_IsKeyPressed[(int)InputDirection::Down] && m_IsKeyPressed[(int)InputDirection::Right])
	{
		m_Direction = AnimationComponent::AnimationState::JumpRightDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightDown);
		ActivateJump();
	}
	else if (m_IsKeyPressed[(int)InputDirection::Down] && m_IsKeyPressed[(int)InputDirection::Left])
	{
		m_Direction = AnimationComponent::AnimationState::JumpLeftDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftDown);
		ActivateJump();
	}
}

void MovementComponent::KeyReleased(InputDirection moveDir)
{
	m_IsKeyPressed[(int)moveDir] = false;
}