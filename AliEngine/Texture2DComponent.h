#pragma once
#include "BaseComponent.h"
#include <SDL.h>
#include "glm\vec2.hpp"
#include <memory>
#include "Texture2D.h"

class Texture2DComponent final : public BaseComponent
{
public:
	Texture2DComponent(const std::string& filename, float scale = 1, bool hasAnimations = false);

	std::shared_ptr<dae::Texture2D> GetTexture2D() const;
	void Render()  override;
	const SDL_Rect& GetSrcRect() const;
	void SetSrcRect(const SDL_Rect& sdlRect);
private:
	std::shared_ptr<dae::Texture2D> m_spTexture2D{};
	glm::vec2 m_Position = {};
	float m_Scale;
	SDL_Rect m_SDLRect;
	bool m_HasAnimations;
};