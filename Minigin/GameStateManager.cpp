#include "MiniginPCH.h"
#include "GameStateManager.h"
#include "../AliEngine/TextComponent.h"
#include "../AliEngine/Texture2DComponent.h"
#include "../AliEngine/TransformComponent.h"
#include "../AliEngine/FPSTextComponent.h"
#include "../AliEngine/BaseCommand.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

using namespace dae;

void GameStateManager::LoadGameMode(dae::Scene::GameMode gameMode)
{
	dae::SceneManager::GetInstance().ClearScene(dae::SceneManager::GetInstance().GetCurrentScene());
	EnemyManager::GetInstance().DeleteAllEnemies();
	CollisionDetectionManager::GetInstance().ClearCollisions();
	dae::SceneManager::GetInstance().GetCurrentScene()->SetCurrentGameMode(gameMode);
	switch (gameMode)
	{
	case dae::Scene::GameMode::SinglePlayer:
		SinglePlayer();
		break;
	case dae::Scene::GameMode::Coop:
		Coop();
		break;
	case dae::Scene::GameMode::Versus:
		Versus();
		break;
	}
}

void GameStateManager::LoadWinScreen()
{
	dae::SceneManager::GetInstance().ClearScene(dae::SceneManager::GetInstance().GetCurrentScene());
	EnemyManager::GetInstance().DeleteAllEnemies();
	CollisionDetectionManager::GetInstance().ClearCollisions();
	//
	auto& scene = SceneManager::GetInstance().CreateScene("Qbert");
	scene.SetCurrentGameMode(dae::Scene::GameMode::None);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//png
	auto losePng = std::make_shared<GameObject>("WinScreen");
	losePng->AddComponent(new TransformComponent(glm::vec2(-350, -200)));
	losePng->AddComponent(new Texture2DComponent("WinScreen.png"));
	scene.Add(losePng);
	//
	auto textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 100)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto text = new TextComponent("Controller : Press Y to go to main menu.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
	//
	textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 150)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	text = new TextComponent("Keyboard : Press U to go to main menu.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
}

void GameStateManager::LoadLoseScreen()
{
	dae::SceneManager::GetInstance().ClearScene(dae::SceneManager::GetInstance().GetCurrentScene());
	EnemyManager::GetInstance().DeleteAllEnemies();
	CollisionDetectionManager::GetInstance().ClearCollisions();
	//
	auto& scene = SceneManager::GetInstance().CreateScene("Qbert");
	scene.SetCurrentGameMode(dae::Scene::GameMode::None);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//png
	auto losePng = std::make_shared<GameObject>("GameOverScreen");
	losePng->AddComponent(new TransformComponent(glm::vec2(-350, -200)));
	losePng->AddComponent(new Texture2DComponent("GameOverScreen.png"));
	scene.Add(losePng);
	//
	auto textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 100)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto text = new TextComponent("Controller : Press Y to go to main menu.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
	//
	textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 150)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	text = new TextComponent("Keyboard : Press U to go to main menu.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
}

void GameStateManager::LoadMenuScreen()
{
	dae::SceneManager::GetInstance().ClearScene(dae::SceneManager::GetInstance().GetCurrentScene());
	EnemyManager::GetInstance().DeleteAllEnemies();
	CollisionDetectionManager::GetInstance().ClearCollisions();
	//
	auto& scene = SceneManager::GetInstance().CreateScene("Qbert");
	scene.SetCurrentGameMode(dae::Scene::GameMode::None);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//gamemodes
	auto textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto text = new TextComponent("Controller : Press X for singleplayer, A for Coop and B for Versus Mode.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
	//
	textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	text = new TextComponent("Keyboard : Press I for singleplayer, O for Coop and P for Versus Mode.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);

	//controls
	textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 150)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	text = new TextComponent("Controller : D pad for movement.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
	//
	textDisplay = std::make_shared<GameObject>("Text");
	textDisplay->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2.0f - 350, m_WindowSurface->h / 2.0f + 200)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	text = new TextComponent("Keyboard : W A S D for movement.", font, SDL_Color{ 255,255,0 });
	textDisplay->AddComponent(text);
	scene.Add(textDisplay);
}

void GameStateManager::SinglePlayer()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	scene->SetCurrentGameMode(dae::Scene::GameMode::SinglePlayer);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//fps counter
	auto go = std::make_shared<GameObject>("FPSCounter");
	auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	go->AddComponent(new FPSTextComponent(font2));
	scene->Add(go);
	////------------------------------------------------------------------------------------------------UI
	//score
	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec2(150, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
	scoreDisplay->AddComponent(scoreCounter);
	scene->Add(scoreDisplay);
	//lives
	auto livesDisplay = std::make_shared<GameObject>("LivesDisplay");
	livesDisplay->AddComponent(new TransformComponent(glm::vec2(250, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto livesCounter = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,255,255 });
	livesDisplay->AddComponent(livesCounter);
	scene->Add(livesDisplay);
	////------------------------------------------------------------------------------------------------GAME
	//level
	auto level = std::make_shared<GameObject>("Pyramid");
	level->AddComponent(new PyramidComponent(glm::vec2(m_WindowSurface->w / 2.0f, m_WindowSurface->h / 2.0f - 10)));
	scene->Add(level);
	scene->AddLevel(level);
	scene->SetCurrentLevel(level);
	//---------------------------------PLAYER
	//q*bert
	const int playerWidth = 16;
	const int playerHeight = 16;
	auto qbert = std::make_shared<GameObject>("Q*Bert");
	qbert->AddComponent(new TransformComponent(glm::vec2(m_WindowSurface->w / 2 + playerWidth, m_WindowSurface->h / 2 - playerHeight), glm::vec2(playerWidth, playerHeight)));
	qbert->AddComponent(new HealthComponent(3));
	qbert->AddComponent(new ScoreComponent(0));
	qbert->AddWatcher(new LivesObserver());
	qbert->AddWatcher(new ScoreObserver());
	qbert->AddComponent(new Texture2DComponent("QBERT.png", 2, true));
	qbert->AddComponent(new PlayerMovementComponent("Q*Bert"));
	qbert->AddComponent(new AnimationComponent(8));
	scene->Add(qbert);
	scene->AddPlayer(qbert);
	CollisionDetectionManager::GetInstance().AddCollisionObject(qbert);
	//player died text
	auto playerDied = std::make_shared<GameObject>("Player 1 Died!");
	playerDied->AddComponent(new TransformComponent(glm::vec2(500, 300)));
	playerDied->AddComponent(new TextComponent("Player 1 Died!", font, SDL_Color{ 255,255,255 }, false));
	scene->Add(playerDied);
	//---------------------------------ENEMIES
	EnemyManager::GetInstance().SetWindowSurface(m_WindowSurface);
	EnemyManager::GetInstance().SetPlayers(qbert);
	EnemyManager::GetInstance().SetLevelObject(level);
}

void GameStateManager::Coop()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	scene->SetCurrentGameMode(dae::Scene::GameMode::Coop);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto go = std::make_shared<GameObject>("FPSCounter");
	auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	go->AddComponent(new FPSTextComponent(font2));
	scene->Add(go);
	//score
	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec2(150, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
	scoreDisplay->AddComponent(scoreCounter);
	scene->Add(scoreDisplay);
	//lives
	auto livesDisplay = std::make_shared<GameObject>("LivesDisplay");
	livesDisplay->AddComponent(new TransformComponent(glm::vec2(250, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto livesCounter = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,255,255 });
	livesDisplay->AddComponent(livesCounter);
	scene->Add(livesDisplay);
	//score2
	auto scoreDisplay2 = std::make_shared<GameObject>("ScoreDisplay2");
	scoreDisplay2->AddComponent(new TransformComponent(glm::vec2(150, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scoreCounter2 = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
	scoreDisplay2->AddComponent(scoreCounter2);
	scene->Add(scoreDisplay2);
	//lives2
	auto livesDisplay2 = std::make_shared<GameObject>("LivesDisplay2");
	livesDisplay2->AddComponent(new TransformComponent(glm::vec2(250, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto livesCounter2 = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,255,255 });
	livesDisplay2->AddComponent(livesCounter2);
	scene->Add(livesDisplay2);

	////------------------------------------------------------------------------------------------------GAME
	//level
	auto level = std::make_shared<GameObject>("Pyramid");
	level->AddComponent(new PyramidComponent(glm::vec2(m_WindowSurface->w / 2.0f, m_WindowSurface->h / 2.0f - 10)));
	scene->Add(level);
	scene->AddLevel(level);
	scene->SetCurrentLevel(level);
	//---------------------------------PLAYER
	//q*bert
	const int playerWidth = 16;
	const int playerHeight = 16;

	glm::vec2 player1SpawnPos = glm::vec2(level->GetComponent<PyramidComponent>()->GetSpecificCube(27)->GetPosition().x + playerWidth, level->GetComponent<PyramidComponent>()->GetSpecificCube(27)->GetPosition().y - playerHeight);
	glm::vec2 player2SpawnPos = glm::vec2(level->GetComponent<PyramidComponent>()->GetSpecificCube(6)->GetPosition().x + playerWidth, level->GetComponent<PyramidComponent>()->GetSpecificCube(6)->GetPosition().y - playerHeight);

	auto qbert = std::make_shared<GameObject>("Q*Bert");
	qbert->AddComponent(new TransformComponent(player1SpawnPos, glm::vec2(playerWidth, playerHeight)));
	qbert->AddComponent(new HealthComponent(3));
	qbert->AddComponent(new ScoreComponent(0));
	qbert->AddWatcher(new LivesObserver());
	qbert->AddWatcher(new ScoreObserver());
	qbert->AddComponent(new Texture2DComponent("QBERT.png", 2, true));
	qbert->AddComponent(new PlayerMovementComponent("Q*Bert", dae::Scene::GameMode::Coop));
	qbert->AddComponent(new AnimationComponent(8));
	scene->Add(qbert);
	scene->AddPlayer(qbert);
	CollisionDetectionManager::GetInstance().AddCollisionObject(qbert);
	//player died text
	auto playerDied = std::make_shared<GameObject>("Player 1 Died!");
	playerDied->AddComponent(new TransformComponent(glm::vec2(500, 150)));
	playerDied->AddComponent(new TextComponent("Player 1 Died!", font, SDL_Color{ 255,255,255 }, false));
	scene->Add(playerDied);
	//q*bert2
	auto qbert2 = std::make_shared<GameObject>("Q*Bert2");
	qbert2->AddComponent(new TransformComponent(player2SpawnPos, glm::vec2(playerWidth, playerHeight)));
	qbert2->AddComponent(new HealthComponent(3));
	qbert2->AddComponent(new ScoreComponent(0));
	qbert2->AddWatcher(new LivesObserver());
	qbert2->AddWatcher(new ScoreObserver());
	qbert2->AddComponent(new Texture2DComponent("QBERT2.png", 2, true));
	qbert2->AddComponent(new PlayerMovementComponent("Q*Bert2", dae::Scene::GameMode::Coop));
	qbert2->AddComponent(new AnimationComponent(8));
	scene->Add(qbert2);
	scene->AddPlayer(qbert2);
	CollisionDetectionManager::GetInstance().AddCollisionObject(qbert2);
	//player 2 died text
	auto player2Died = std::make_shared<GameObject>("Player 2 Died!");
	player2Died->AddComponent(new TransformComponent(glm::vec2(500, 150)));
	player2Died->AddComponent(new TextComponent("Player 2 Died!", font, SDL_Color{ 255,255,255 }, false));
	scene->Add(player2Died);
	//---------------------------------ENEMIES
	EnemyManager::GetInstance().SetWindowSurface(m_WindowSurface);
	EnemyManager::GetInstance().SetPlayers(qbert);
	EnemyManager::GetInstance().SetLevelObject(level);
}

void GameStateManager::Versus()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	scene->SetCurrentGameMode(dae::Scene::GameMode::Versus);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto go = std::make_shared<GameObject>("FPSCounter");
	auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	go->AddComponent(new FPSTextComponent(font2));
	scene->Add(go);

	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec2(150, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
	scoreDisplay->AddComponent(scoreCounter);
	scene->Add(scoreDisplay);
	//lives
	auto livesDisplay = std::make_shared<GameObject>("LivesDisplay");
	livesDisplay->AddComponent(new TransformComponent(glm::vec2(250, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto livesCounter = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,255,255 });
	livesDisplay->AddComponent(livesCounter);
	scene->Add(livesDisplay);
	////------------------------------------------------------------------------------------------------GAME
	//level
	auto level = std::make_shared<GameObject>("Pyramid");
	level->AddComponent(new PyramidComponent(glm::vec2(m_WindowSurface->w / 2.0f, m_WindowSurface->h / 2.0f - 10)));
	scene->Add(level);
	scene->AddLevel(level);
	scene->SetCurrentLevel(level);
	//---------------------------------PLAYER
	//q*bert
	const int playerWidth = 16;
	const int playerHeight = 16;
	glm::vec2 qBertSpawnPos = glm::vec2(level->GetComponent<PyramidComponent>()->GetSpecificCube(27)->GetPosition().x + playerWidth, level->GetComponent<PyramidComponent>()->GetSpecificCube(27)->GetPosition().y - playerHeight);
	auto qbert = std::make_shared<GameObject>("Q*Bert");
	qbert->AddComponent(new TransformComponent(qBertSpawnPos, glm::vec2(playerWidth, playerHeight)));
	qbert->AddComponent(new HealthComponent(3));
	qbert->AddComponent(new ScoreComponent(0));
	qbert->AddWatcher(new LivesObserver());
	qbert->AddWatcher(new ScoreObserver());
	qbert->AddComponent(new Texture2DComponent("QBERT.png", 2, true));
	qbert->AddComponent(new PlayerMovementComponent("Q*Bert", dae::Scene::GameMode::Versus));
	qbert->AddComponent(new AnimationComponent(8));
	scene->Add(qbert);
	scene->AddPlayer(qbert);
	CollisionDetectionManager::GetInstance().AddCollisionObject(qbert);
	//-----------------------------------COILY VERSUS
	int enemyWidth = 16;
	int enemyHeight = 48;
	glm::vec2 coilySpawnPos = glm::vec2(level->GetComponent<PyramidComponent>()->GetSpecificCube(0)->GetPosition().x + enemyWidth, level->GetComponent<PyramidComponent>()->GetSpecificCube(0)->GetPosition().y - enemyHeight);
	auto coily = std::make_shared<GameObject>("Coily");
	coily->AddComponent(new TransformComponent(coilySpawnPos, glm::vec2(enemyWidth, enemyHeight)));
	coily->AddComponent(new Texture2DComponent("CoilyPlayer.png", 2, true));
	coily->AddComponent(new PlayerMovementComponent("Coily", dae::Scene::GameMode::Versus, true));
	coily->AddComponent(new AnimationComponent(8));
	scene->Add(coily);
	scene->AddPlayer(coily);
	CollisionDetectionManager::GetInstance().AddCollisionObject(coily);
	//player died text
	auto playerDied = std::make_shared<GameObject>("Player 1 Died!");
	playerDied->AddComponent(new TransformComponent(glm::vec2(500, 300)));
	playerDied->AddComponent(new TextComponent("Player 1 Died!", font, SDL_Color{ 255,255,255 }, false));
	scene->Add(playerDied);
	//---------------------------------ENEMIES
	EnemyManager::GetInstance().SetWindowSurface(m_WindowSurface);
	EnemyManager::GetInstance().SetPlayers(qbert);
	EnemyManager::GetInstance().SetLevelObject(level);
}