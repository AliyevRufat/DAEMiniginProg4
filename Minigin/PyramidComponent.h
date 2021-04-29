#pragma once
#include "ComponentIncludes.h"
#include "GameObject.h"
#include "Scene.h"
#include <glm\vec2.hpp>

class PyramidComponent final : public BaseComponent
{
public:
	PyramidComponent(dae::Scene& scene, const glm::vec3& topCubePos);

private:
	void Initialize(dae::Scene& scene);
	void CreateMap(dae::Scene& scene);
	void CreateCube(const size_t& index, const glm::vec3& pos, dae::Scene& scene);

private:
	const static int m_MaxCubes = 28;
	const int m_FirstRowCubeCount;
	const int m_CubeColumnCount;
	const SDL_Rect m_CubeSrcRect;
	glm::vec3 m_CubeOffset;
	const glm::vec3 m_HighestCubePos;
	const float m_CubeScale;

	std::shared_ptr<GameObject> m_Cubes[m_MaxCubes];
};
