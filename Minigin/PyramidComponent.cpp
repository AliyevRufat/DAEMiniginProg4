#pragma once
#include "MiniginPCH.h"
#include "PyramidComponent.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "Texture2DComponent.h"
#include "SceneManager.h"

PyramidComponent::PyramidComponent(const glm::vec2& topCubePos)
	:m_FirstRowCubeCount(7)
	, m_CubeColumnCount(7)
	, m_CubeSrcRect{ 0,160,32,32 }
	, m_CubeDistance{ 16,24 }
	, m_HighestCubePos{ topCubePos }
	, m_CubeScale{ 2.0f }
{
	int mostLeftBlockIndex = 0;
	int lowestBlockIndex = 6;
	for (int i = 0; i < m_SideLength; i++)
	{
		m_MostLeftBlocks[i] = mostLeftBlockIndex;
		mostLeftBlockIndex += m_SideLength - i;

		m_MostRightBlocks[i] = i;

		m_LowestBlocks[i] = lowestBlockIndex; // +1 because its the bottom
		lowestBlockIndex += 6 - i;
	}
	m_CubeDistance.x *= dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale();
	m_CubeDistance.y *= dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale();
	Initialize();
}

void PyramidComponent::Initialize()
{
	CreateMap();
}

void PyramidComponent::CreateMap()
{
	int indexCounter = 0;
	int rowCubeCount = m_FirstRowCubeCount;
	glm::vec2 highestCubePos = m_HighestCubePos;

	for (size_t j = 0; j < m_CubeColumnCount; j++)
	{
		for (size_t i = 0; i < rowCubeCount; i++)
		{
			glm::vec2 pos = highestCubePos;
			pos.x += m_CubeDistance.x * i;
			pos.y += m_CubeDistance.y * i;

			CreateCube(indexCounter, pos);
			indexCounter++;
		}
		highestCubePos.x -= m_CubeDistance.x;
		highestCubePos.y += m_CubeDistance.y;

		rowCubeCount--;
	}
}

void PyramidComponent::CreateCube(const size_t& index, const glm::vec2& pos)
{
	auto cube = std::make_shared<CubeObject>();
	cube->GetGameObject()->AddComponent(new TransformComponent(pos, glm::vec2(m_CubeScale, m_CubeScale)));
	cube->GetGameObject()->AddComponent(new Texture2DComponent("Cubes.png", m_CubeScale, true));
	cube->GetGameObject()->AddComponent(new AnimationComponent(3));
	m_Cubes[index] = cube;
}

int PyramidComponent::GetColumnNumber(const int& currentTileIndex) const
{
	int cubeCount = m_FirstRowCubeCount;

	for (int i = 0; i < m_CubeColumnCount; i++)
	{
		if (currentTileIndex < cubeCount)
			return i;

		cubeCount += (m_FirstRowCubeCount - (i + 1)); // + i because it's not an index but a count
	}

	return -1;
}

bool PyramidComponent::GetNextCubeIndex(int& currentIndex, AnimationComponent::AnimationState jumpDir) const
{
	int columnIndex = GetColumnNumber(currentIndex);

	switch (jumpDir)
	{
	case AnimationComponent::AnimationState::JumpLeftDown:
	{
		for (size_t i = 0; i < m_SideLength; i++)
		{
			if (currentIndex == m_LowestBlocks[i])
			{
				return false;
			}
		}
		currentIndex += m_FirstRowCubeCount - columnIndex;
		break;
	}
	case AnimationComponent::AnimationState::JumpRightTop:
	{
		for (size_t i = 0; i < m_SideLength; i++)
		{
			if (currentIndex == m_MostRightBlocks[i])
			{
				return false;
			}
		}
		int columnIndexAfterJump = columnIndex - 1;
		currentIndex -= m_FirstRowCubeCount - columnIndexAfterJump;
		break;
	}
	case AnimationComponent::AnimationState::JumpLeftTop:
	{
		for (size_t i = 0; i < m_SideLength; i++)
		{
			if (currentIndex == m_MostLeftBlocks[i])
			{
				return false;
			}
		}
		currentIndex--;
		break;
	}
	case AnimationComponent::AnimationState::JumpRightDown:
	{
		for (size_t i = 0; i < m_SideLength; i++)
		{
			if (currentIndex == m_LowestBlocks[i])
			{
				return false;
			}
		}
		currentIndex++;
		break;
	}
	}

	return true;
}

void PyramidComponent::Update()
{
	for (auto& cube : m_Cubes)
	{
		if (cube)
		{
			cube->Update();
		}
	}
}

void PyramidComponent::Render()
{
	for (const auto& cube : m_Cubes)
	{
		if (cube)
		{
			cube->Render();
		}
	}
}

const glm::vec2& PyramidComponent::GetCubeOffset() const
{
	return m_CubeDistance;
}