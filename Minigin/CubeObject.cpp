#include "MiniginPCH.h"
#include "CubeObject.h"
#include <memory>
#include "Texture2DComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "Transform.h"
#include "SceneManager.h"

using namespace dae;

CubeObject::CubeObject()
	: m_ColorState{ ColorState::FirstColor }
	, m_CurrentLevel{ dae::SceneManager::GetInstance().GetCurrentScene()->GetGameLevel() }
	, m_MaxColorSwitchTime{ 0.3f }
	, m_CurrentColorSwitchTime{ 0.0f }
	, m_HasDiscNextToIt{ false }
{
	m_pGameObject = std::make_shared<GameObject>(("Cube"));
}

void CubeObject::ChangeColor()
{
	if (m_CurrentLevel == Scene::Level::FirstLevel)
	{
		if (m_ColorState == ColorState::FirstColor)
		{
			m_ColorState = ColorState::SecondColor;

			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Yellow);
		}
	}
	else if (m_CurrentLevel == Scene::Level::SecondLevel)
	{
		if (m_ColorState == ColorState::FirstColor)
		{
			m_ColorState = ColorState::SecondColor;

			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Yellow);
		}
		else if (m_ColorState == ColorState::SecondColor)
		{
			m_ColorState = ColorState::ThirdColor;

			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Blue);
		}
	}
	else if (m_CurrentLevel == Scene::Level::ThirdLevel)
	{
		if (m_ColorState == ColorState::FirstColor)
		{
			m_ColorState = ColorState::SecondColor;

			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Yellow);
		}
		else if (m_ColorState == ColorState::SecondColor)
		{
			m_ColorState = ColorState::FirstColor;

			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Red);
		}
	}
	m_CurrentLevel = dae::SceneManager::GetInstance().GetCurrentScene()->GetGameLevel();
}

void CubeObject::ResetColor()
{
	m_ColorState = ColorState::FirstColor;
	m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Red);
}

bool CubeObject::GetHasDiscNextToIt() const
{
	return m_HasDiscNextToIt;
}

void CubeObject::SetHasDiscNextToIt(bool hasDisc)
{
	m_HasDiscNextToIt = hasDisc;
}

void CubeObject::Update()
{
	m_pGameObject->Update();
}

void CubeObject::Render()
{
	m_pGameObject->Render();
}

void CubeObject::ColorFlash()
{
	m_CurrentColorSwitchTime += Time::GetInstance().GetDeltaTime();

	if (m_CurrentColorSwitchTime >= m_MaxColorSwitchTime)
	{
		int nextColor = 0;

		if (m_pGameObject->GetComponent<AnimationComponent>()->GetCubeColorState() != AnimationComponent::CubeColorState::Blue)
		{
			nextColor = int(m_pGameObject->GetComponent<AnimationComponent>()->GetCubeColorState()) + 1;
		}

		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState(nextColor));

		m_CurrentColorSwitchTime -= m_CurrentColorSwitchTime;
	}
}

const glm::vec2& CubeObject::GetPosition() const
{
	return m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
}