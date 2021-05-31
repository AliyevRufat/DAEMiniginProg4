#pragma once
#include "ComponentIncludes.h"

class Texture2DComponent final : public BaseComponent
{
public:
	Texture2DComponent(const std::string& filename, float scale = 1);

	std::shared_ptr<dae::Texture2D> GetTexture2D() const;
	void Render()  override;
	const SDL_Rect& GetSrcRect() const;
private:
	std::shared_ptr<dae::Texture2D> m_spTexture2D{};
	glm::vec2 m_Position = {};
	float m_Scale;
	SDL_Rect m_SDLRect;
};