#include "MiniginPCH.h"
#include "MovementComponent.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "PyramidComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Time.h"

MovementComponent::MovementComponent()
	:m_Jumped{ false }
	, m_JumpingUp{ false }
	, m_CurrentPos{}
	, m_DesiredPosLeft{}
	, m_DesiredPosRight{}
	, m_DesiredPosTop{}
	, m_DesiredPosBottom{}
	, m_JumpDirection{}
	, m_MoveDirection{}
	, m_JumpSpeed{ 25 }
	, m_IsKeyPressed{ false }
{
	const glm::vec3& cubeOffset = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<PyramidComponent>()->GetCubeOffset();
	m_MoveDistance = cubeOffset;
}

void MovementComponent::Update()
{
	//if (m_Jumped)
	//{
	//	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();
	//	glm::vec3 pos = transform->GetTransform().GetPosition();
	//
	//	const float moveDistanceRatio = (m_MoveDistance.y / m_MoveDistance.x);
	//	float jumpHeight = m_MoveDistance.y / 2.0f;
	//	const glm::vec2 speed = { m_JumpSpeed,m_JumpSpeed * moveDistanceRatio * (m_MoveDistance.y / jumpHeight) };
	//
	//	pos.x += Time::GetInstance().GetDeltaTime() * speed.x * m_JumpDirection.x;
	//
	//	if (m_JumpDirection.y >= 0)
	//		jumpHeight = m_MoveDistance.y / 2.0f;
	//	else
	//		jumpHeight = m_MoveDistance.y * 1.5f;
	//
	//	if (m_FirstHalfOfTheJump)
	//	{
	//		pos.y -= deltaT * speed.y;
	//
	//		if (abs(pos.y - m_JumpStartPos.y) > jumpHeight)
	//			m_FirstHalfOfTheJump = false;
	//	}
	//	else
	//	{
	//		pos.y += deltaT * speed.y;
	//	}
	//
	//	if (abs(pos.x - m_JumpStartPos.x) > m_MoveDistance.x)
	//	{
	//		// Done with jumping
	//		const auto& renderComp = m_pGameObject->GetComponent<RenderComponent>();
	//		const auto& playerComp = m_pGameObject->GetComponent<PlayerComponent>();
	//
	//		// Direction - 1 = not jumping version of sprite
	//		renderComp->SetSrcRect(SDL_Rect{ ((int)m_Direction - 1) * (int)playerComp->GetSize().x,0,(int)playerComp->GetSize().x,(int)playerComp->GetSize().y });
	//		m_Jumped = false;
	//	}
	//
	//	transform->SetPosition(pos.x, pos.y);
	//}
	//

	//if (m_IsKeyPressed[(int)m_MoveDirection] && m_IsKeyPressed[(int)m_MoveDirection])
	//{
	//}
	if (m_Jumped)
	{
		m_pGameObject->GetComponent<TransformComponent>()->SetPosition(m_CurrentPos);

		if (m_JumpingUp)
		{
			m_CurrentPos.y += (200 * Time::GetInstance().GetDeltaTime()) * m_JumpDirection.y;

			if (m_CurrentPos.y <= m_DesiredPosTop - 20)
			{
				m_JumpingUp = false;
			}
		}
		else
		{
			m_CurrentPos.y -= (200 * Time::GetInstance().GetDeltaTime()) * m_JumpDirection.y;
		}
		m_CurrentPos.x += (110 * Time::GetInstance().GetDeltaTime()) * m_JumpDirection.x;

		if (m_CurrentPos.x <= m_DesiredPosLeft)
		{
			m_CurrentPos.x = m_DesiredPosLeft;
		}
		//stop jump
		if (m_CurrentPos.x == m_DesiredPosLeft && m_CurrentPos.y >= m_DesiredPosTop && !m_JumpingUp)
		{
			m_Jumped = false;
			//set to idle
			//int moveDirIdle = (int)m_MoveDirection;
			//moveDirIdle--;
			//m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState(int(moveDirIdle)));
		}
	}
}

void MovementComponent::Move(MoveDirection moveDir)
{
	m_CurrentPos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	//

	m_DesiredPosLeft = m_CurrentPos.x;
	m_DesiredPosRight = m_CurrentPos.x;
	m_DesiredPosTop = m_CurrentPos.y;
	m_DesiredPosBottom = m_CurrentPos.y;

	m_DesiredPosLeft -= 80;
	m_DesiredPosRight += 80;
	m_DesiredPosTop -= 120;
	m_DesiredPosBottom += 120;
	//
	//
	m_MoveDirection = moveDir;
	m_IsKeyPressed[(int)moveDir] = true;

	if (m_IsKeyPressed[(int)MoveDirection::Up] && m_IsKeyPressed[(int)MoveDirection::Left])
	{
		m_JumpDirection.x = -1;
		m_JumpDirection.y = -1;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState::JumpLeftTop);
		m_Jumped = true;
		m_JumpingUp = true;
	}
	else if (m_IsKeyPressed[(int)MoveDirection::Up] && m_IsKeyPressed[(int)MoveDirection::Right])
	{
		m_JumpDirection.x = 1;
		m_JumpDirection.y = -1;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState::JumpRightTop);
		m_Jumped = true;
		m_JumpingUp = true;
	}
	else if (m_IsKeyPressed[(int)MoveDirection::Down] && m_IsKeyPressed[(int)MoveDirection::Right])
	{
		m_JumpDirection.x = 1;
		m_JumpDirection.y = 1;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState::JumpRightDown);
		m_Jumped = true;
		m_JumpingUp = true;
	}
	else if (m_IsKeyPressed[(int)MoveDirection::Down] && m_IsKeyPressed[(int)MoveDirection::Left])
	{
		m_JumpDirection.x = -1;
		m_JumpDirection.y = 1;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationState::JumpLeftDown);
		m_Jumped = true;
		m_JumpingUp = true;
	}
}

void MovementComponent::KeyReleased(MoveDirection moveDir)
{
	m_IsKeyPressed[(int)moveDir] = false;
}