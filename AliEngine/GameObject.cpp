#include "pch.h"
#include "../AliEngine/BaseComponent.h"
#include "../AliEngine/GameObject.h"
#include "../AliEngine/ResourceManager.h"
#include "../AliEngine/Renderer.h"
#include "../AliEngine/EngineTime.h"
#include "Subject.h"

GameObject::GameObject(const std::string& name)
	:SceneObject(name), m_pActorChanged{ new Subject() }
{
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		delete m_pComponents[i];
	}
}

void GameObject::Update()
{
	if (!m_IsActive)
	{
		return;
	}
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Update();
	}
}

void GameObject::Render() const
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Render();
	}
}

void GameObject::AddComponent(BaseComponent* pComponent)
{
	pComponent->SetOwnerObject(this);
	m_pComponents.push_back(pComponent);
}

void GameObject::AddWatcher(Observer* pObserver)
{
	m_pActorChanged->AddObserver(pObserver);
}

void GameObject::Notify(Event event)
{
	m_pActorChanged->Notify(this, event);
}