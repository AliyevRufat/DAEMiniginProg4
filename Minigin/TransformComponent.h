#pragma once
#include "ComponentIncludes.h"

class TransformComponent final : public BaseComponent
{
public:
	TransformComponent(const glm::vec2& pos, const glm::vec2& scale = glm::vec2{ 1,1 });

	dae::Transform GetTransform() const;
	void SetPosition(const glm::vec2& position);
	void SetScale(const glm::vec2& scale);
	const SDL_Rect& GetRect();
private:
	dae::Transform m_Transform;
	SDL_Rect m_SDLRect;
};