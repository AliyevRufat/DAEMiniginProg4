#include "MiniginPCH.h"
#include "EnemyMovementComponent.h"
#include "../AliEngine/TransformComponent.h"
#include "../AliEngine/Texture2DComponent.h"
#include "EnemyManager.h"
#include "../AliEngine/SceneManager.h"
#include "PyramidComponent.h"

EnemyMovementComponent::EnemyMovementComponent(EnemyManager::EnemyType enemyType, const std::shared_ptr<GameObject>& pPlayer, const std::shared_ptr<GameObject>& pPlayer2)
	:m_pPlayer{ pPlayer }
	, m_pPlayer2{ pPlayer2 }
	, m_CurrentJumpTime{ 0.0f }
	, m_MaxJumpTime{ 1.5f }
	, m_EnemyType{ enemyType }
	, m_SpeedMultiplier{ EnemyManager::GetInstance().GetEnemySpeedMultiplier() }
	, m_IsDead{ false }
	, m_IsOnSpawnCube{ false }
{
	m_Speed *= m_SpeedMultiplier;
	m_MaxJumpTime -= m_SpeedMultiplier / 4.0f;
	if (m_EnemyType == EnemyManager::EnemyType::Coily)
	{
		m_CurrentColumn = 1;
		m_CurrentRow = 1;
		m_CurrentCubeIndex = 1;
		m_SourceHeightOffset = -8;
	}
	else if (m_EnemyType == EnemyManager::EnemyType::Sam)
	{
		m_CurrentColumn = 0;
		m_CurrentRow = 1;
		m_CurrentCubeIndex = 7;
		m_SourceHeightOffset = 0;
	}
	else if (m_EnemyType == EnemyManager::EnemyType::Slick)
	{
		m_CurrentColumn = 1;
		m_CurrentRow = 1;
		m_CurrentCubeIndex = 1;
		m_SourceHeightOffset = 0;
	}
	else if (m_EnemyType == EnemyManager::EnemyType::WrongWay)
	{
		m_CurrentColumn = 0;
		m_CurrentRow = 6;
		m_CurrentCubeIndex = 27;
		m_SourceHeightOffset = 0;
	}
	else if (m_EnemyType == EnemyManager::EnemyType::Ugg)
	{
		m_CurrentColumn = 6;
		m_CurrentRow = 6;
		m_CurrentCubeIndex = 6;
		m_SourceHeightOffset = 0;
	}
}

void EnemyMovementComponent::Update()
{
	if (!m_IsOnSpawnCube)
	{
		if (!IsOnSpawnCube())
		{
			return;
		}
	}

	if (m_EnemyType == EnemyManager::EnemyType::WrongWay || m_EnemyType == EnemyManager::EnemyType::Ugg)
	{
		if (m_IsFalling)
		{
			SidewaysFall();
		}
		else if (m_IsMoving)
		{
			SidewaysJump();
		}
	}
	else
	{
		BaseMovementComponent::Update();
	}

	m_CurrentJumpTime += EngineTime::GetInstance().GetDeltaTime();

	if (m_CurrentJumpTime >= m_MaxJumpTime)
	{
		if (m_EnemyType == EnemyManager::EnemyType::Coily)
		{
			FollowPlayer();
		}
		else if (m_EnemyType == EnemyManager::EnemyType::Sam || m_EnemyType == EnemyManager::EnemyType::Slick)
		{
			Descend();
		}
		else if (m_EnemyType == EnemyManager::EnemyType::WrongWay || m_EnemyType == EnemyManager::EnemyType::Ugg)
		{
			SidewaysMovement();
		}

		m_CurrentJumpTime -= m_CurrentJumpTime;
	}
}

bool EnemyMovementComponent::IsOnSpawnCube()
{
	const auto& currentMap = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>();
	auto pos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();
	auto deltaTime = EngineTime::GetInstance().GetDeltaTime();
	const float posOffset = 400.0f;
	const int srcOffset = 40;

	switch (m_EnemyType)
	{
	case EnemyManager::EnemyType::Coily:
		if (pos.y < currentMap->GetSpecificCube(1)->GetPosition().y - srcOffset)
		{
			transform->SetPosition(glm::vec2(pos.x, pos.y + posOffset * deltaTime));
		}
		else
		{
			m_IsOnSpawnCube = true;
			return true;
		}
		break;
	case EnemyManager::EnemyType::Sam:
		if (pos.y < currentMap->GetSpecificCube(7)->GetPosition().y)
		{
			transform->SetPosition(glm::vec2(pos.x, pos.y + posOffset * deltaTime));
		}
		else
		{
			m_IsOnSpawnCube = true;
			return true;
		}

		break;
	case EnemyManager::EnemyType::Slick:
		if (pos.y < currentMap->GetSpecificCube(1)->GetPosition().y)
		{
			transform->SetPosition(glm::vec2(pos.x, pos.y + posOffset * deltaTime));
		}
		else
		{
			m_IsOnSpawnCube = true;
			return true;
		}
		break;
	case EnemyManager::EnemyType::Ugg:
		if (pos.x > currentMap->GetSpecificCube(6)->GetPosition().x + srcOffset)
		{
			transform->SetPosition(glm::vec2(pos.x - posOffset * deltaTime, pos.y));
		}
		else
		{
			m_IsOnSpawnCube = true;
			return true;
		}
		break;
	case EnemyManager::EnemyType::WrongWay:
		if (pos.x < currentMap->GetSpecificCube(27)->GetPosition().x)
		{
			transform->SetPosition(glm::vec2(pos.x + posOffset * deltaTime, pos.y));
		}
		else
		{
			m_IsOnSpawnCube = true;
			return true;
		}
		break;
	}
	return false;
}

void EnemyMovementComponent::FollowPlayer()
{
	if (m_IsMoving)
	{
		return;
	}

	const int yOffset = 30;
	auto playerPos = m_pPlayer->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	auto pos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();

	if (playerPos.x <= pos.x && playerPos.y - yOffset <= pos.y) // left top
	{
		m_Direction = AnimationComponent::AnimationState::JumpLeftTop;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftTop);
		ActivateJump();
		--m_CurrentColumn;
		--m_CurrentRow;
	}
	else if (playerPos.x >= pos.x && playerPos.y - yOffset <= pos.y) // right top
	{
		m_Direction = AnimationComponent::AnimationState::JumpRightTop;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightTop);
		ActivateJump();
		--m_CurrentRow;
	}
	else if (playerPos.x <= pos.x && playerPos.y >= pos.y) // left bottom
	{
		m_Direction = AnimationComponent::AnimationState::JumpLeftDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftDown);
		ActivateJump();
		++m_CurrentRow;
	}
	else if (playerPos.x >= pos.x && playerPos.y >= pos.y) // right down
	{
		m_Direction = AnimationComponent::AnimationState::JumpRightDown;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightDown);
		ActivateJump();
		++m_CurrentColumn;
		++m_CurrentRow;
	}
}

void EnemyMovementComponent::Descend()
{
	if (m_IsMoving)
	{
		return;
	}

	int randNr = rand() % 2;

	if (randNr == 0)
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
}

void EnemyMovementComponent::SidewaysMovement()
{
	if (m_IsMoving)
	{
		return;
	}
	int randNr = rand() % 2;

	if (m_EnemyType == EnemyManager::EnemyType::WrongWay)
	{
		if (randNr == 0)
		{
			m_Direction = AnimationComponent::AnimationState::JumpRightDown;
			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightDown);
			ActivateJump(true);
			++m_CurrentColumn;
		}
		else
		{
			m_Direction = AnimationComponent::AnimationState::JumpRightTop;
			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpRightTop);
			ActivateJump(true);
			--m_CurrentRow;
		}
	}
	else
	{
		if (randNr == 0)
		{
			m_Direction = AnimationComponent::AnimationState::JumpLeftDown;
			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftDown);
			ActivateJump(true);
			--m_CurrentColumn;
		}
		else
		{
			m_Direction = AnimationComponent::AnimationState::JumpLeftTop;
			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState::JumpLeftTop);
			ActivateJump(true);
			--m_CurrentColumn;
			--m_CurrentRow;
		}
	}
}

void EnemyMovementComponent::SidewaysJump()
{
	float deltaTime = EngineTime::GetInstance().GetDeltaTime();
	//
	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();
	glm::vec2 pos = transform->GetTransform().GetPosition();

	const float moveDistanceRatio = (m_MoveDistance.y / m_MoveDistance.x);
	float jumpHeight = m_MoveDistance.y / 2.0f;

	glm::vec2 speed = { m_Speed,m_Speed * moveDistanceRatio * (m_MoveDistance.y / jumpHeight) };

	if (m_Direction == AnimationComponent::AnimationState::JumpRightDown || m_Direction == AnimationComponent::AnimationState::JumpLeftDown)
	{
		speed.x *= 2;
	}

	if (m_Direction == AnimationComponent::AnimationState::JumpRightDown || m_Direction == AnimationComponent::AnimationState::JumpRightTop)
		pos.x += deltaTime * speed.x;
	else
		pos.x -= deltaTime * speed.x;

	if ((int)m_Direction >= (int)AnimationComponent::AnimationState::IdleRightDown)
		jumpHeight = m_MoveDistance.y / 2.0f;
	else
		jumpHeight = m_MoveDistance.y * 1.5f;

	if (m_Direction != AnimationComponent::AnimationState::JumpRightDown && m_Direction != AnimationComponent::AnimationState::JumpLeftDown)
	{
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
	}

	if (abs(pos.x - m_JumpStartPos.x) > m_MoveDistance.x)
	{
		//landed on cube
		int NonJumpingSprite = (int)m_Direction - 1;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::AnimationState(NonJumpingSprite));
		const auto& CurrentMap = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>();

		auto cube = CurrentMap->GetSpecificCube(m_CurrentCubeIndex);

		//offset fix
		auto cubePos = cube->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		auto srcRect = m_pGameObject->GetComponent<Texture2DComponent>()->GetSrcRect();

		if (m_EnemyType == EnemyManager::EnemyType::WrongWay)
		{
			m_pGameObject->GetComponent<TransformComponent>()->SetPosition(glm::vec2(cubePos.x - srcRect.w, cubePos.y + srcRect.h * 2 + m_SourceHeightOffset));
		}
		else
		{
			m_pGameObject->GetComponent<TransformComponent>()->SetPosition(glm::vec2(cubePos.x + srcRect.w * 2.5f, cubePos.y + srcRect.h * 2 + m_SourceHeightOffset));
		}

		m_IsMoving = false;
	}
	else
	{
		transform->SetPosition(pos);
	}
}

void EnemyMovementComponent::SidewaysFall()
{
	float deltaTime = EngineTime::GetInstance().GetDeltaTime();

	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();
	glm::vec2 pos = transform->GetTransform().GetPosition();

	if (m_EnemyType == EnemyManager::EnemyType::WrongWay)
	{
		pos.x += deltaTime * m_Speed * 4;
	}
	else
	{
		pos.x -= deltaTime * m_Speed * 4;
	}

	transform->SetPosition(pos);

	if (m_EnemyType == EnemyManager::EnemyType::WrongWay)
	{
		if (pos.x >= 1300)
		{
			m_IsOffScreen = true;
		}
	}
	else if (m_EnemyType == EnemyManager::EnemyType::WrongWay)
	{
		if (pos.x <= 0)
		{
			m_IsOffScreen = true;
		}
	}
}