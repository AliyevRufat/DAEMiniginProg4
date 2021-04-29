#pragma once
#include "ComponentIncludes.h"

class TransformComponent final : public BaseComponent
{
public:
	TransformComponent(const glm::vec3& pos);

	dae::Transform GetTransform() const;
	void SetPosition(const glm::vec3& position);
private:
	dae::Transform m_Transform;
};