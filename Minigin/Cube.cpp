#include "MiniginPCH.h"
#include "Cube.h"
#include <memory>
#include "../AliEngine/Texture2DComponent.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/TransformComponent.h"
#include "AnimationComponent.h"
#include "../AliEngine/Transform.h"
#include "../AliEngine/SceneManager.h"

using namespace dae;

Cube::Cube()
	: m_ColorState{ ColorState::FirstColor }
	, m_CurrentLevel{ dae::SceneManager::GetInstance().GetCurrentScene()->GetGameLevel() }
	, m_MaxColorSwitchTime{ 0.3f }
	, m_CurrentColorSwitchTime{ 0.0f }
	, m_HasDiscNextToIt{ false }
{
	m_pGameObject = std::make_shared<GameObject>(("Cube"));
}

void Cube::ChangeColor(bool& hasColored)
{
	if (m_CurrentLevel == Scene::Level::FirstLevel)
	{
		if (m_ColorState == ColorState::FirstColor)
		{
			m_ColorState = ColorState::SecondColor;

			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Yellow);
			hasColored = true;
		}
	}
	else if (m_CurrentLevel == Scene::Level::SecondLevel)
	{
		if (m_ColorState == ColorState::FirstColor)
		{
			m_ColorState = ColorState::SecondColor;

			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Yellow);
			hasColored = true;
		}
		else if (m_ColorState == ColorState::SecondColor)
		{
			m_ColorState = ColorState::ThirdColor;

			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Blue);
			hasColored = true;
		}
	}
	else if (m_CurrentLevel == Scene::Level::ThirdLevel)
	{
		if (m_ColorState == ColorState::FirstColor)
		{
			m_ColorState = ColorState::SecondColor;

			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Yellow);
			hasColored = true;
		}
		else if (m_ColorState == ColorState::SecondColor)
		{
			m_ColorState = ColorState::FirstColor;

			m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Red);
			hasColored = true;
		}
	}
	m_CurrentLevel = dae::SceneManager::GetInstance().GetCurrentScene()->GetGameLevel();
}

void Cube::ResetColor()
{
	m_ColorState = ColorState::FirstColor;
	m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Red);
}

bool Cube::GetHasDiscNextToIt() const
{
	return m_HasDiscNextToIt;
}

void Cube::SetHasDiscNextToIt(bool hasDisc)
{
	m_HasDiscNextToIt = hasDisc;
}

void Cube::Update()
{
	m_pGameObject->Update();
}

void Cube::Render()
{
	m_pGameObject->Render();
}

void Cube::ColorFlash()
{
	m_CurrentColorSwitchTime += EngineTime::GetInstance().GetDeltaTime();

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

glm::vec2 Cube::GetPosition() const
{
	return m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
}