#pragma once
#include "ComponentIncludes.h"
#include "GameObject.h"
#include "Scene.h"
#include <glm\vec2.hpp>
#include "CubeObject.h"
#include "AnimationComponent.h"

class PyramidComponent final : public BaseComponent
{
public:
	PyramidComponent(const glm::vec2& topCubePos);

	void Update() override;
	void Render()  override;

	const glm::vec2& GetCubeOffset() const;
	std::shared_ptr<CubeObject> GetCube(int index) { return m_Cubes[index]; }
	bool GetNextCubeIndex(int& currentIndex, AnimationComponent::AnimationState jumpDir, bool isSidewaysJump, int currentColumn, int currentRow) const; // Returns false if the player jumps off the map

private:
	void Initialize();
	void CreateMap();
	void CreateCube(const size_t& index, const glm::vec2& pos);

private:
	const static int m_MaxCubes = 28;
	const int m_RowAmount;
	const SDL_Rect m_CubeSrcRect;
	glm::vec2 m_CubeDistance;
	const glm::vec2 m_HighestCubePos;
	const float m_CubeScale;

	std::shared_ptr<CubeObject> m_Cubes[m_MaxCubes];
};
