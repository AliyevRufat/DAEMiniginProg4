#include "MiniginPCH.h"
#include "BaseMovementComponent.h"
#include "../AliEngine/TransformComponent.h"
#include "AnimationComponent.h"
#include "../AliEngine/SceneManager.h"
#include "PyramidComponent.h"
#include "../AliEngine/SceneManager.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/EngineTime.h"
#include "../AliEngine/Texture2DComponent.h"

BaseMovementComponent::BaseMovementComponent()
	: m_Speed{ 100 }
	, m_IsMoving{ false }
	, m_CurrentCubeIndex{ 0 }
	, m_IsFalling{ false }
	, m_Direction{ AnimationComponent::AnimationState::IdleLeftDown }
	, m_FirstHalfOfTheJump{}
	, m_JumpStartPos{}
	, m_SourceHeightOffset{}
	, m_CurrentColumn{ 0 }
	, m_CurrentRow{ 0 }
	, m_IsOffScreen{ false }
{
	const glm::vec2& cubeOffset = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>()->GetCubeOffset();
	m_MoveDistance = cubeOffset;
}

void BaseMovementComponent::Update()
{
	if (m_IsFalling)
	{
		FallToDeath();
	}
	else if (m_IsMoving)
	{
		Jump();
	}
}

void BaseMovementComponent::ActivateJump(bool isSideWaysJump)
{
	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();

	m_JumpStartPos = transform->GetTransform().GetPosition();
	m_IsMoving = true;
	m_FirstHalfOfTheJump = true;

	const auto& CurrentMap = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>();

	if (!CurrentMap->GetNextCubeIndex(m_CurrentCubeIndex, m_Direction, isSideWaysJump, m_CurrentColumn, m_CurrentRow))
	{
		m_IsFalling = true;
	}
	else
	{
		m_IsFalling = false;
	}
}

void BaseMovementComponent::Jump()
{
	float deltaTime = EngineTime::GetInstance().GetDeltaTime();
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
		//landed on cube
		int NonJumpingSprite = (int)m_Direction - 1;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState(NonJumpingSprite));
		const auto& CurrentMap = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>();

		auto cube = CurrentMap->GetSpecificCube(m_CurrentCubeIndex);

		if (m_pGameObject->GetName() == "Q*Bert") // TODO : should not be here , add it to player movement component or something
		{
			cube->ChangeColor();
			//TODO : add score
		}
		else if (m_pGameObject->GetName() == "Q*Bert2") // TODO : should not be here , add it to player movement component or something
		{
			cube->ChangeColor();
			//TODO : add score
		}
		else if (m_pGameObject->GetName() == "Sam" || m_pGameObject->GetName() == "Slick")
		{
			cube->ResetColor();
		}
		//offset fix
		auto cubePos = cube->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		auto srcRect = m_pGameObject->GetComponent<Texture2DComponent>()->GetSrcRect();

		m_pGameObject->GetComponent<TransformComponent>()->SetPosition(glm::vec2(cubePos.x + srcRect.w, cubePos.y - srcRect.h + m_SourceHeightOffset));

		m_IsMoving = false;
	}
	else
	{
		transform->SetPosition(pos);
	}
}

void BaseMovementComponent::FallToDeath()
{
	float deltaTime = EngineTime::GetInstance().GetDeltaTime();
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

	if (pos.y > 720)
	{
		int NonJumpingSprite = (int)m_Direction - 1;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState(NonJumpingSprite));
		m_IsMoving = false;
		m_IsFalling = false;
		m_IsOffScreen = true;
	}
	else
	{
		transform->SetPosition(pos);
	}
}