#pragma once
#include "MiniginPCH.h"
#include "PyramidComponent.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "Texture2DComponent.h"
#include "SceneManager.h"

PyramidComponent::PyramidComponent(const glm::vec2& topCubePos)
	:m_RowAmount(7)
	, m_CubeSrcRect{ 0,160,32,32 }
	, m_CubeDistance{ 16,24 }
	, m_HighestCubePos{ topCubePos }
	, m_CubeScale{ 2.0f }
{
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
	int rowCubeCount = m_RowAmount;
	glm::vec2 highestCubePos = m_HighestCubePos;

	for (size_t j = 0; j < m_RowAmount; j++)
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

bool PyramidComponent::GetNextCubeIndex(int& currentIndex, AnimationComponent::AnimationState jumpDir, bool isSidewaysJump, int currentColumn, int currentRow) const
{
	if (isSidewaysJump)
	{
		switch (jumpDir)
		{
		case AnimationComponent::AnimationState::JumpLeftDown:
		{
			if (currentColumn == 0)
			{
				return false;
			}
			currentIndex += m_RowAmount - currentRow + currentColumn - 1;
			break;
		}
		case AnimationComponent::AnimationState::JumpRightTop:
		{
			if (currentColumn == currentRow)
			{
				return false;
			}
			currentIndex -= m_RowAmount - currentRow + currentColumn + 1;
			break;
		}
		case AnimationComponent::AnimationState::JumpLeftTop:
		{
			if (currentColumn == 0)
			{
				return false;
			}
			--currentIndex;
			break;
		}
		case AnimationComponent::AnimationState::JumpRightDown:
		{
			if (currentColumn == currentRow)
			{
				return false;
			}
			currentIndex -= m_RowAmount - currentRow + currentColumn;
			break;
		}
		}

		return true;
	}
	else
	{
		switch (jumpDir)
		{
		case AnimationComponent::AnimationState::JumpLeftDown:
		{
			if (currentRow == m_RowAmount - 1)
			{
				return false;
			}
			currentIndex += m_RowAmount - currentRow + currentColumn;
			break;
		}
		case AnimationComponent::AnimationState::JumpRightTop:
		{
			if (currentColumn == currentRow)
			{
				return false;
			}
			currentIndex -= m_RowAmount - currentRow + currentColumn + 1;
			break;
		}
		case AnimationComponent::AnimationState::JumpLeftTop:
		{
			if (currentColumn == 0)
			{
				return false;
			}
			currentIndex--;
			break;
		}
		case AnimationComponent::AnimationState::JumpRightDown:
		{
			if (currentRow == m_RowAmount - 1)
			{
				return false;
			}
			currentIndex++;
			break;
		}
		}

		return true;
	}
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