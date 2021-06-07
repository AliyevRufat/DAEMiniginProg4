#pragma once
#include "../AliEngine/Singleton.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/InputManager.h"
#include "../AliEngine/SceneManager.h"
#include "../AliEngine/Renderer.h"
#include "../AliEngine/ResourceManager.h"
#include <SDL.h>
#include "../AliEngine/GameObject.h"
#include "../AliEngine/Scene.h"
#include "../AliEngine/EngineTime.h"
#include "ScoreObserver.h"
#include "LivesObserver.h"
#include "../AliEngine/AudioService.h"
#include "../AliEngine/Locator.h"
#include "AnimationComponent.h"
#include "../AliEngine/ConsoleAudioService.h"
#include "PyramidComponent.h"
#include "PlayerMovementComponent.h"
#include "EnemyMovementComponent.h"
#include "CollisionDetectionManager.h"

class GameStateManager final : public dae::Singleton<GameStateManager>
{
public:
	void LoadGameMode(dae::Scene::GameMode gameMode);
	void LoadWinScreen();
	void LoadLoseScreen();
	void LoadMenuScreen();
	void SetWindowSurface(SDL_Surface* windowSurface) { m_WindowSurface = windowSurface; };
private:
	void SinglePlayer();
	void Coop();
	void Versus();
	SDL_Surface* m_WindowSurface;
	friend class dae::Singleton<GameStateManager>;
	GameStateManager() = default;
};
