#pragma once
#include "MiniginPCH.h"
#include "PyramidComponent.h"
#include "../AliEngine/Renderer.h"
#include "../AliEngine/TransformComponent.h"
#include "../AliEngine/Texture2DComponent.h"
#include "HealthComponent.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/EngineTime.h"
#include "../AliEngine/SceneManager.h"
#include "GameStateManager.h"
#include "ScoreComponent.h"
#include "CollisionDetectionManager.h"
#include "PlayerMovementComponent.h"
#include "PyramidLoader.h"
#include <iomanip>
#include "EnemyManager.h"

PyramidComponent::PyramidComponent(const glm::vec2& topCubePos)
	:m_RowAmount(7)
	, m_CubeSrcRect{ 0,160,32,32 }
	, m_CubeDistance{ 16,24 }
	, m_HighestCubePos{ topCubePos }
	, m_CubeScale{ 2.0f }
	, m_MaxCubesColorChangeTime{ 3.0f }
	, m_CurrentCubesColorChangeTime{ 0.0f }
	, m_IsLevelFinished{ false }
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
	PyramidLoader pyramidLoader("../Data/PyramidCoordinations.txt");
	std::vector<glm::vec3>cubePositions = pyramidLoader.ReadGetPositions();

	for (size_t i = 0; i < cubePositions.size(); i++)
	{
		CreateCube(i, glm::vec3(cubePositions[i].x * m_CubeScale + m_HighestCubePos.x, cubePositions[i].y * m_CubeScale + m_HighestCubePos.y, 0));
	}

	SpawnDiscs();
}

void PyramidComponent::CreateCube(const size_t& index, const glm::vec2& pos)
{
	auto cube = std::make_shared<Cube>();
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

	for (auto& disc : m_Discs)
	{
		if (disc)
		{
			disc->Update();
		}
	}
	DeleteUsedDiscs();

	auto currScene = dae::SceneManager::GetInstance().GetCurrentScene();

	if (m_IsLevelFinished)
	{
		m_CurrentCubesColorChangeTime += EngineTime::GetInstance().GetDeltaTime();
		CubesColorChangeOnLevelComplete();

		if (currScene->AreAllObjectsActive())
		{
			currScene->SetObjectsIsActive(false);
		}
	}
	else
	{
		if (!currScene->AreAllObjectsActive())
		{
			currScene->SetObjectsIsActive(true);
		}
		m_IsLevelFinished = LevelCompletedCheck();
		if (m_IsLevelFinished)
		{
			if (dae::SceneManager::GetInstance().GetCurrentScene()->GetGameLevel() == dae::Scene::Level::Finished)
			{
				GameStateManager::GetInstance().LoadWinScreen();
			}
			EnemyManager::GetInstance().SetParametersAccordingToTheLevel(currScene->GetGameLevel());
			EnemyManager::GetInstance().DeleteAllEnemies();
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

void PyramidComponent::TeleportPlayersToCorrectPos(dae::Scene::GameMode gameMode)
{
	EnemyManager::GetInstance().DeleteAllEnemies();

	const int playerWidth = 16;
	const int playerHeight = 16;
	if (gameMode == dae::Scene::GameMode::SinglePlayer)
	{
		auto cube = GetSpecificCube(0);

		auto player = dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0);

		glm::vec2 startingPosition{ 0,0 };

		startingPosition.x = cube->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().x + (dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale() * 8.f);
		startingPosition.y = cube->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().y - (dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale() * 10.f);

		player->GetComponent<TransformComponent>()->SetPosition(startingPosition);
		player->GetComponent<PlayerMovementComponent>()->SetCubeIndexColumnAndRow(0, 0, 0);
	}
	else if (gameMode == dae::Scene::GameMode::Coop)
	{
		auto currentScene = dae::SceneManager::GetInstance().GetCurrentScene();
		auto player1 = currentScene->GetPlayer(0);
		auto player2 = currentScene->GetPlayer(1);

		glm::vec2 desPosPlayer1{ 0,0 };
		glm::vec2 desPosPlayer2{ 0,0 };

		if (player1->GetComponent<PlayerMovementComponent>()->GetIsOnDisc())
		{
			desPosPlayer1.x = GetSpecificCube(0)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().x + playerWidth;
			desPosPlayer1.y = GetSpecificCube(0)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().y - playerHeight;
			player1->GetComponent<TransformComponent>()->SetPosition(desPosPlayer1);
			player1->GetComponent<PlayerMovementComponent>()->SetCubeIndexColumnAndRow(0, 0, 0);

			desPosPlayer2.x = GetSpecificCube(6)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().x + playerWidth;
			desPosPlayer2.y = GetSpecificCube(6)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().y - playerHeight;

			player2->GetComponent<TransformComponent>()->SetPosition(desPosPlayer2);
			player2->GetComponent<PlayerMovementComponent>()->SetCubeIndexColumnAndRow(6, 6, 6);
		}
		else if (player2->GetComponent<PlayerMovementComponent>()->GetIsOnDisc())
		{
			desPosPlayer2.x = GetSpecificCube(0)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().x + playerWidth;
			desPosPlayer2.y = GetSpecificCube(0)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().y - playerHeight;
			player2->GetComponent<TransformComponent>()->SetPosition(desPosPlayer2);
			player2->GetComponent<PlayerMovementComponent>()->SetCubeIndexColumnAndRow(0, 0, 0);

			desPosPlayer1.x = GetSpecificCube(27)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().x + playerWidth;
			desPosPlayer1.y = GetSpecificCube(27)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().y - playerHeight;
			player1->GetComponent<TransformComponent>()->SetPosition(desPosPlayer1);
			player1->GetComponent<PlayerMovementComponent>()->SetCubeIndexColumnAndRow(27, 0, 6);
		}
		else if (!player1->GetComponent<PlayerMovementComponent>()->GetIsOnDisc() && !player2->GetComponent<PlayerMovementComponent>()->GetIsOnDisc())
		{
			desPosPlayer1.x = GetSpecificCube(27)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().x + playerWidth;
			desPosPlayer1.y = GetSpecificCube(27)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().y - playerHeight;
			player1->GetComponent<TransformComponent>()->SetPosition(desPosPlayer1);
			player1->GetComponent<PlayerMovementComponent>()->SetCubeIndexColumnAndRow(27, 0, 6);
			desPosPlayer2.x = GetSpecificCube(6)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().x + playerWidth;
			desPosPlayer2.y = GetSpecificCube(6)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().y - playerHeight;
			player2->GetComponent<TransformComponent>()->SetPosition(desPosPlayer2);
			player2->GetComponent<PlayerMovementComponent>()->SetCubeIndexColumnAndRow(6, 6, 6);
		}
	}
	else
	{
		auto currentScene = dae::SceneManager::GetInstance().GetCurrentScene();
		auto player1 = currentScene->GetPlayer(0);
		auto player2 = currentScene->GetPlayer(1);

		glm::vec2 desPosPlayer1{ 0,0 };
		glm::vec2 desPosPlayer2{ 0,0 };

		desPosPlayer1.x = GetSpecificCube(27)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().x + playerWidth;
		desPosPlayer1.y = GetSpecificCube(27)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().y - playerHeight;
		player1->GetComponent<TransformComponent>()->SetPosition(desPosPlayer1);
		player1->GetComponent<PlayerMovementComponent>()->SetCubeIndexColumnAndRow(27, 0, 6);

		desPosPlayer2.x = GetSpecificCube(0)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().x + playerWidth;
		desPosPlayer2.y = GetSpecificCube(0)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().y - playerHeight;

		player2->GetComponent<TransformComponent>()->SetPosition(desPosPlayer2);
		player2->GetComponent<PlayerMovementComponent>()->SetCubeIndexColumnAndRow(0, 0, 0);
	}
}

std::shared_ptr<Cube> PyramidComponent::GetSpecificCube(int index) const
{
	return m_Cubes[index];
}

std::shared_ptr<FlyingDisc> PyramidComponent::GetDisc(std::shared_ptr<GameObject> gameObject)
{
	return *std::find_if(m_Discs.begin(), m_Discs.end(), [&](const std::shared_ptr<FlyingDisc>& x)
	{
		return x->GetGameObject() == gameObject;
	});
}

void PyramidComponent::SpawnDiscs()
{
	//add score for the amount of unused discs
	for (size_t i = 0; i < m_Discs.size(); i++)
	{
		dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0).get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::DiscLeftAtEndOfTheStage);
		auto player2 = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
		if (player2)
		{
			player2.get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::DiscLeftAtEndOfTheStage);
		}
	}

	//
	int index = 0;
	int row = 7;
	std::vector<int> leftSideIndices;
	std::vector<int> rightSideIndices;
	for (int i = 0; i < 7; i++)
	{
		index += i;

		leftSideIndices.push_back(i * row + index);

		if (i != 0) rightSideIndices.push_back(i);

		row--;
	}

	int leftSideCubeIndex = leftSideIndices[rand() % 5 + 2];
	int rightSideCubeIndex = rightSideIndices[rand() % 4 + 2];

	glm::vec2 disc1Pos = m_Cubes[leftSideCubeIndex]->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();

	glm::vec2 disc2Pos = m_Cubes[rightSideCubeIndex]->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();

	disc1Pos.x -= m_CubeDistance.x * 1.5f;
	disc2Pos.x += m_CubeDistance.x * 2.5f;

	for (auto& cube : m_Cubes)
	{
		cube->SetHasDiscNextToIt(false);
	}
	for (auto& disc : m_Discs)
	{
		disc->SetIsUsed(true);
	}

	CreateDisc(disc1Pos, *dae::SceneManager::GetInstance().GetCurrentScene());
	m_Cubes[leftSideCubeIndex]->SetHasDiscNextToIt(true);

	CreateDisc(disc2Pos, *dae::SceneManager::GetInstance().GetCurrentScene());
	m_Cubes[rightSideCubeIndex]->SetHasDiscNextToIt(true);
}

void PyramidComponent::DeleteUsedDiscs()
{
	if (m_Discs.size() > 0)
	{
		for (size_t i = 0; i < m_Discs.size(); i++)
		{
			if (m_Discs[i])
			{
				if (m_Discs[i]->GetIsUsed())
				{
					m_Discs[i]->GetGameObject()->SetMarkForDelete(true);
					CollisionDetectionManager::GetInstance().DeleteGameObject(m_Discs[i]->GetGameObject());
					m_Discs.erase(m_Discs.begin() + i);
				}
			}
		}
	}
}

bool PyramidComponent::LevelCompletedCheck()
{
	auto currentScene = dae::SceneManager::GetInstance().GetCurrentScene();

	if (currentScene->GetGameLevel() == dae::Scene::Level::FirstLevel)
	{
		for (auto& cube : m_Cubes)
		{
			if (cube->GetCurrentColorState() != Cube::ColorState::SecondColor)
			{
				return false;
			}
		}

		currentScene->SetGameLevel(dae::Scene::Level(int(currentScene->GetGameLevel()) + 1));

		for (auto& cube : m_Cubes)
		{
			cube->ResetColor();
		}
		Locator::GetAudio().QueueSound(AudioService::SoundIds::VictoryEffect, true, 50);

		return true;
	}
	else if (currentScene->GetGameLevel() == dae::Scene::Level::SecondLevel)
	{
		for (auto& cube : m_Cubes)
		{
			if (cube->GetCurrentColorState() != Cube::ColorState::ThirdColor)
			{
				return false;
			}
		}

		currentScene->SetGameLevel(dae::Scene::Level(int(currentScene->GetGameLevel()) + 1));

		for (auto& cube : m_Cubes)
		{
			cube->ResetColor();
		}
		Locator::GetAudio().QueueSound(AudioService::SoundIds::VictoryEffect, true, 50);

		return true;
	}
	else if (currentScene->GetGameLevel() == dae::Scene::Level::ThirdLevel)
	{
		for (auto& cube : m_Cubes)
		{
			if (cube->GetCurrentColorState() != Cube::ColorState::SecondColor)
			{
				return false;
			}
		}

		currentScene->SetGameLevel(dae::Scene::Level(3));

		for (auto& cube : m_Cubes)
		{
			cube->ResetColor();
		}
		Locator::GetAudio().QueueSound(AudioService::SoundIds::VictoryEffect, true, 50);

		return true;
	}
	return false;
}

void PyramidComponent::CubesColorChangeOnLevelComplete()
{
	for (auto& cube : m_Cubes)
	{
		cube->ColorFlash();
	}

	if (m_CurrentCubesColorChangeTime >= m_MaxCubesColorChangeTime)
	{
		m_CurrentCubesColorChangeTime -= m_CurrentCubesColorChangeTime;
		SpawnDiscs();
		TeleportPlayersToCorrectPos(dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode());
		m_IsLevelFinished = false;
		for (auto& cube : m_Cubes)
		{
			cube->ResetColor();
		}
	}
}

void PyramidComponent::CreateDisc(const glm::vec2& pos, dae::Scene& scene)
{
	m_Discs.push_back(std::make_shared<FlyingDisc>(m_Cubes[0]->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition()));
	auto newDisc = m_Discs[m_Discs.size() - 1]->GetGameObject();

	int discWidth = 16;
	int discHeight = 17;

	newDisc->AddComponent(new TransformComponent(pos, glm::vec2{ discWidth, discHeight }));
	newDisc->AddComponent(new Texture2DComponent("Disc.png", m_CubeScale, true));
	newDisc->AddComponent(new AnimationComponent(5));
	CollisionDetectionManager::GetInstance().AddCollisionObject(newDisc);
	scene.Add(newDisc);
}