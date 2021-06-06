#pragma once
#include "ComponentIncludes.h"
#include "../AliEngine/GameObject.h"
#include "../AliEngine/Scene.h"
#include <glm\vec2.hpp>
#include "CubeObject.h"
#include "AnimationComponent.h"
#include "CubeObject.h"
#include "FlyingDisc.h"

class PyramidComponent final : public BaseComponent
{
public:
	PyramidComponent(const glm::vec2& topCubePos);

	void Update() override;
	void Render()  override;

	const glm::vec2& GetCubeOffset() const;
	bool GetNextCubeIndex(int& currentIndex, AnimationComponent::AnimationState jumpDir, bool isSidewaysJump, int currentColumn, int currentRow) const; // Returns false if the player jumps off the map
	void TeleportPlayersToCorrectPos(dae::Scene::GameMode gameMode);
	std::shared_ptr<CubeObject> GetSpecificCube(int index) const;
	std::shared_ptr<FlyingDisc> GetDisc(std::shared_ptr<GameObject> gameObject);
private:
	void Initialize();
	void CreateMap();
	void CreateCube(const size_t& index, const glm::vec2& pos);
	void SpawnDiscs();
	void CreateDisc(const glm::vec2& pos, dae::Scene& scene);
	void DeleteUsedDiscs();
	bool LevelCompletedCheck();
	void CubesColorChangeOnLevelComplete();
	//
	const static int m_MaxCubes = 28;
	const int m_RowAmount;
	const SDL_Rect m_CubeSrcRect;
	glm::vec2 m_CubeDistance;
	const glm::vec2 m_HighestCubePos;
	const float m_CubeScale;

	std::shared_ptr<CubeObject> m_Cubes[m_MaxCubes];
	std::vector<std::shared_ptr<FlyingDisc>> m_Discs;

	const float m_MaxCubesColorChangeTime; //time length of cube flashing
	float m_CurrentCubesColorChangeTime;
	bool m_IsLevelFinished;
};
