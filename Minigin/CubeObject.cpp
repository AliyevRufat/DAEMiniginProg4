#include "MiniginPCH.h"
#include "CubeObject.h"
#include <memory>
#include "Texture2DComponent.h"
#include "AnimationComponent.h"

using namespace dae;

CubeObject::CubeObject()
	: m_Colored{ false }
{
	m_pGameObject = std::make_shared<GameObject>(("Cube"));
}

void CubeObject::Color()
{
	if (!m_Colored)
	{
		m_Colored = true;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Yellow);
	}
}

void CubeObject::ColorBack()
{
	if (m_Colored)
	{
		m_Colored = false;
		m_pGameObject->GetComponent<AnimationComponent>()->SetAnimationState(AnimationComponent::CubeColorState::Red);
	}
}

void CubeObject::Update()
{
	m_pGameObject->Update();
}

void CubeObject::Render()
{
	m_pGameObject->Render();
}