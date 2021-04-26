#pragma once
#include "ComponentIncludes.h"

class Texture2DComponent final : public BaseComponent
{
public:
	Texture2DComponent(const std::string& filename);

	std::shared_ptr<dae::Texture2D> GetTexture2D() const;
	void Render() override;
private:
	std::shared_ptr<dae::Texture2D> m_spTexture2D{};
	glm::vec3 m_Position = {};
};