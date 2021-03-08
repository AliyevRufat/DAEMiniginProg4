#pragma once

class GameObject;

class BaseComponent
{
public:
	BaseComponent() = default;
	virtual ~BaseComponent() = default;
	virtual void Update() {};
	virtual void Render() {};
	virtual void SetOwnerObject(GameObject* pGameObject) { m_pGameObject = pGameObject; }
protected:
	GameObject* m_pGameObject = nullptr;
	bool m_IsInitialized = false;
};
