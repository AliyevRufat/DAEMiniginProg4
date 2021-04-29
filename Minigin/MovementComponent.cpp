#include "MiniginPCH.h"
#include "MovementComponent.h"
#include "TransformComponent.h"
#include "Time.h"

MovementComponent::MovementComponent()
	:m_Jumped{ false }
	, m_JumpingUp{ false }
	, m_MoveDirection{}
	, m_CurrentPos{}
	, m_DesiredPosLeft{}
	, m_DesiredPosRight{}
	, m_DesiredPosTop{}
	, m_DesiredPosBottom{}
{
}

void MovementComponent::Update()
{
	//
	if (m_Jumped)
	{
		switch (m_MoveDirection)
		{
		case MoveDirection::LeftTop:

			m_pGameObject->GetComponent<TransformComponent>()->SetPosition(m_CurrentPos);

			if (m_JumpingUp)
			{
				m_CurrentPos.y -= 200 * Time::GetInstance().GetDeltaTime();

				if (m_CurrentPos.y <= m_DesiredPosTop - 20)
				{
					m_JumpingUp = false;
				}
			}
			else
			{
				m_CurrentPos.y += 200 * Time::GetInstance().GetDeltaTime();
			}
			m_CurrentPos.x -= 110 * Time::GetInstance().GetDeltaTime();

			if (m_CurrentPos.x <= m_DesiredPosLeft)
			{
				m_CurrentPos.x = m_DesiredPosLeft;
			}
			//stop jump
			if (m_CurrentPos.x == m_DesiredPosLeft && m_CurrentPos.y >= m_DesiredPosTop && !m_JumpingUp)
			{
				m_Jumped = false;
			}
			break;
		case MoveDirection::RightTop:

			m_pGameObject->GetComponent<TransformComponent>()->SetPosition(m_CurrentPos);

			if (m_JumpingUp)
			{
				m_CurrentPos.y -= 200 * Time::GetInstance().GetDeltaTime();

				if (m_CurrentPos.y <= m_DesiredPosTop - 20)
				{
					m_JumpingUp = false;
				}
			}
			else
			{
				m_CurrentPos.y += 200 * Time::GetInstance().GetDeltaTime();
			}
			m_CurrentPos.x += 110 * Time::GetInstance().GetDeltaTime();

			if (m_CurrentPos.x >= m_DesiredPosRight)
			{
				m_CurrentPos.x = m_DesiredPosRight;
			}
			//stop jump
			if (m_CurrentPos.x == m_DesiredPosRight && m_CurrentPos.y >= m_DesiredPosTop && !m_JumpingUp)
			{
				m_Jumped = false;
			}
			break;
		case MoveDirection::RightDown:

			m_pGameObject->GetComponent<TransformComponent>()->SetPosition(m_CurrentPos);

			if (m_JumpingUp)
			{
				m_CurrentPos.y += 200 * Time::GetInstance().GetDeltaTime();

				if (m_CurrentPos.y >= m_DesiredPosBottom)
				{
					m_JumpingUp = false;
				}
			}
			else
			{
				m_CurrentPos.y -= 200 * Time::GetInstance().GetDeltaTime();
			}
			m_CurrentPos.x += 110 * Time::GetInstance().GetDeltaTime();

			if (m_CurrentPos.x >= m_DesiredPosRight)
			{
				m_CurrentPos.x = m_DesiredPosRight;
			}
			//stop jump
			if (m_CurrentPos.x == m_DesiredPosRight && m_CurrentPos.y >= m_DesiredPosBottom && !m_JumpingUp)
			{
				m_Jumped = false;
			}
			break;
		case MoveDirection::LeftDown:

			m_pGameObject->GetComponent<TransformComponent>()->SetPosition(m_CurrentPos);

			if (m_JumpingUp)
			{
				m_CurrentPos.y += 200 * Time::GetInstance().GetDeltaTime();

				if (m_CurrentPos.y >= m_DesiredPosBottom)
				{
					m_JumpingUp = false;
				}
			}
			else
			{
				m_CurrentPos.y -= 200 * Time::GetInstance().GetDeltaTime();
			}
			m_CurrentPos.x -= 110 * Time::GetInstance().GetDeltaTime();

			if (m_CurrentPos.x <= m_DesiredPosLeft)
			{
				m_CurrentPos.x = m_DesiredPosLeft;
			}
			//stop jump
			if (m_CurrentPos.x == m_DesiredPosLeft && m_CurrentPos.y >= m_DesiredPosBottom && !m_JumpingUp)
			{
				m_Jumped = false;
			}
			break;
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
	m_MoveDirection = moveDir;
	m_Jumped = true;
	m_JumpingUp = true;
}