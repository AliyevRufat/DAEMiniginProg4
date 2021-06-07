#include "MiniginPCH.h"
#include "FlyingDisc.h"
#include "../AliEngine/SceneManager.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/EngineTime.h"
#include "../AliEngine/Transform.h"
#include "EnemyManager.h"
#include "AnimationComponent.h"
#include "PlayerMovementComponent.h"
#include <cmath>

FlyingDisc::FlyingDisc(const glm::vec2& finalPos)
	:m_FinalPos{ finalPos }
{
	m_pGameObject = std::make_shared<GameObject>(("Disc"));

	m_FinalPos.y -= 25 * dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale();
	m_FinalPos.x += 5 * dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale();
}

void FlyingDisc::Update()
{
	m_pGameObject->Update();
	SwitchColors();
	MoveToTheTop();
}

void FlyingDisc::Render() const
{
	m_pGameObject->Render();
}

void FlyingDisc::SetIsUsed(bool isUsed)
{
	m_IsUsed = isUsed;
}

bool FlyingDisc::GetIsUsed() const
{
	return 	m_IsUsed;
}

void FlyingDisc::SwitchColors()
{
	if (m_AnimTimer > 0)
	{
		m_AnimTimer -= EngineTime::GetInstance().GetDeltaTime();
	}
	else
	{
		auto animComp = m_pGameObject->GetComponent<AnimationComponent>();

		if (int(animComp->GetAnimationState()) < int(animComp->GetNrOfColumns()) - 1)
		{
			animComp->SetAnimationState((AnimationComponent::AnimationState(int(animComp->GetAnimationState()) + 1)));
		}
		else
		{
			animComp->SetAnimationState(AnimationComponent::AnimationState::Cycle);
		}
		m_AnimTimer = m_AnimTime;
	}
}

void FlyingDisc::MoveToTheTop()
{
	if (m_IsMovingToTop)
	{
		if (m_Direction == glm::vec2{ 0,0 })
		{
			auto dist = m_FinalPos - m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();

			auto length = sqrt((dist.x * dist.x) + (dist.y * dist.y));

			m_Direction.x = dist.x / length;
			m_Direction.y = dist.y / length;
		}

		m_pTransformComponent = m_pGameObject->GetComponent<TransformComponent>();

		glm::vec2 discPosition = m_pTransformComponent->GetTransform().GetPosition();

		if (abs(discPosition.x - m_FinalPos.x) > 2)
		{
			auto newPos = discPosition + m_Direction * m_Speed * EngineTime::GetInstance().GetDeltaTime();

			m_pTransformComponent->SetPosition(newPos);
		}
		else
		{
			m_pTransformComponent->SetPosition(glm::vec3{ m_FinalPos.x, m_FinalPos.y, 0 });
			m_IsMovingToTop = false;
			m_IsUsed = true;
			dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<PlayerMovementComponent>()->SetDiscTransform(nullptr);

			auto player2 = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(1);
			if (player2)
			{
				player2->GetComponent<PlayerMovementComponent>()->SetDiscTransform(nullptr);
			}
		}
	}
}