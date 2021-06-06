#include "pch.h"
#include "../AliEngine/FPSTextComponent.h"
#include "../AliEngine/TransformComponent.h"
#include "../AliEngine/TextComponent.h"
#include "../AliEngine/GameObject.h"
#include "../AliEngine/EngineTime.h"
#include <iostream>
#include <SDL.h>
#include <string>

FPSTextComponent::FPSTextComponent(const std::shared_ptr<dae::Font>& spFont)
	: m_pText{ new TextComponent(m_FPS, spFont , SDL_Color{ 255,255,0 }) }
{
}

FPSTextComponent::~FPSTextComponent()
{
	delete m_pText;
}

void FPSTextComponent::Update()
{
	if (!m_IsInitialized && m_pGameObject->GetComponent<TransformComponent>()) // doonce
	{
		m_IsInitialized = true;
		m_pText->SetPosition(m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition());
	}
	//
	m_FPS = "FPS: " + std::to_string(int(1.f / EngineTime::GetInstance().GetDeltaTime()));
	m_pText->UpdateText(m_FPS);
};

void FPSTextComponent::Render()
{
	m_pText->Render();
}