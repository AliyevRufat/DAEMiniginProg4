#pragma once
#include "../AliEngine/GameObject.h"
#include "glm\vec2.hpp"
#include "../AliEngine/TransformComponent.h"

class FlyingDisc
{
public:
	FlyingDisc(const glm::vec2& finalPos);

	FlyingDisc(const FlyingDisc& other) = delete;
	FlyingDisc(FlyingDisc&& other) = delete;
	FlyingDisc& operator=(const FlyingDisc& other) = delete;
	FlyingDisc& operator=(FlyingDisc&& other) = delete;
	~FlyingDisc() = default;

	void Update();
	void Render() const;

	std::shared_ptr<GameObject> GetGameObject() const { return m_pGameObject; };

	void SetIsMovingToTop(bool isMoving) { m_IsMovingToTop = isMoving; }

	bool GetIsUsed() const;
	void SetIsUsed(bool isUsed);

private:
	void SwitchColors();
	void MoveToTheTop();

	std::shared_ptr<GameObject> m_pGameObject = nullptr;
	TransformComponent* m_pTransformComponent = nullptr;

	float m_AnimTimer = 0;
	float m_AnimTime = 0.2f;

	float m_MoveFactor = 0.f;
	float m_Speed = 130.0f;

	glm::vec2 m_FinalPos;
	glm::vec2 m_Direction = glm::vec2{ 0,0 };

	bool m_IsMovingToTop = false;
	bool m_IsUsed = false;
};
