#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "Time.h"
#include "ScoreObserver.h"
#include "LivesObserver.h"
#include "AudioService.h"
#include "Locator.h"
#include "AnimationComponent.h"
#include "ConsoleAudioService.h"
#include "PyramidComponent.h"
#include "PlayerMovementComponent.h"
#include "EnemyMovementComponent.h"
#include "CollisionDetectionManager.h"
#include "Commands.h"

using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init_Video Error: ") + SDL_GetError());
	}

	if (SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init_Audio Error: ") + SDL_GetError());
	}

	int frequency = 44100;
	int chunkSize = 2048;
	int channels = 2;

	if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunkSize) < 0)
		throw std::runtime_error(std::string("SDL_Audio Error: ") + Mix_GetError());

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280,
		720,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	Renderer::GetInstance().Init(m_Window);

	Locator::Provide(new ConsoleAudioService());
	//Locator::GetAudio().AddSound(AudioService::SoundIds::AttackEffect, "Resources/Sounds/Attack.wav", true);
	//Locator::GetAudio().AddSound(AudioService::SoundIds::MainMenuStream, "Resources/Sounds/Music.mp3", false);
	//Locator::GetAudio().QueueSound(AudioService::SoundIds::MainMenuStream, false, 100);
	//Locator::GetAudio().QueueSound(AudioService::SoundIds::AttackEffect, true, 100);
}

void dae::Minigin::BindCommands()
{
	auto& inputManager = InputManager::GetInstance();

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		//assign buttons
		inputManager.AssignKey<DieCommand>(ControllerButton::ButtonA, i);
		inputManager.AssignKey<IncreasePointsCommand>(ControllerButton::ButtonB, i);
		inputManager.AssignKey<DieCommand>(ControllerButton::ButtonX, i);
		inputManager.AssignKey<IncreasePointsCommand>(ControllerButton::ButtonY, i);
		//move
		inputManager.AssignKey<JumpUp>(ControllerButton::ButtonUp, i);
		inputManager.AssignKey<JumpDown>(ControllerButton::ButtonDown, i);
		inputManager.AssignKey<JumpLeft>(ControllerButton::ButtonLeft, i);
		inputManager.AssignKey<JumpRight>(ControllerButton::ButtonRight, i);
		//keyboard
		inputManager.AssignKey<JumpUp>(KeyboardButton::W, i);
		inputManager.AssignKey<JumpDown>(KeyboardButton::S, i);
		inputManager.AssignKey<JumpLeft>(KeyboardButton::A, i);
		inputManager.AssignKey<JumpRight>(KeyboardButton::D, i);
		//
		inputManager.AssignKey<ExitCommand>(ControllerButton::ButtonSelect, i);
		inputManager.AssignKey<FartCommand>(ControllerButton::ButtonStart, i);
		inputManager.AssignKey<FartCommand>(ControllerButton::ButtonLeftThumb, i);
		inputManager.AssignKey<FartCommand>(ControllerButton::ButtonRightThumb, i);
		inputManager.AssignKey<FartCommand>(ControllerButton::ButtonLeftShoulder, i);
		inputManager.AssignKey<FartCommand>(ControllerButton::ButtonRightShoulder, i);
	}
	////assign triggers
	//inputManager.AssignTrigger<AimCommand>(m_Triggers[0].first);
	//inputManager.AssignTrigger<ShootCommand>(m_Triggers[1].first);
	////assign sticks
	//inputManager.AssignStick<MoveCommand>(m_Sticks[0].first);
	//inputManager.AssignStick<LookCommand>(m_Sticks[1].first);
}

void dae::Minigin::Cleanup()
{
	Locator::FreeResources();
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	Mix_Quit();
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();
	//
	auto& renderer{ Renderer::GetInstance() };
	auto& sceneManager{ SceneManager::GetInstance() };
	auto& input{ InputManager::GetInstance() };

	bool doContinue{ true };
	auto lastTime{ high_resolution_clock::now() };

	std::thread soundThread(&AudioService::Update, &Locator::GetAudio());

	BindCommands();

	while (doContinue)
	{
		const auto currentTime{ high_resolution_clock::now() };
		const float deltaTime{ duration<float>(currentTime - lastTime).count() };
		lastTime = currentTime;

		input.ProcessInput();
		input.ControllerAnalogs();
		input.InputHandler();
		doContinue = input.KeyboardInput();

		Time::GetInstance().SetDeltaTime(deltaTime);

		sceneManager.Update();
		renderer.Render();
		CollisionDetectionManager::GetInstance().Update();
	}

	soundThread.detach();

	Cleanup();
}

void dae::Minigin::LoadSinglePlayerScene() const
{
	SDL_Surface* windowSurface = SDL_GetWindowSurface(m_Window);
	auto& scene = SceneManager::GetInstance().CreateScene("Qbert");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	{
		///background
		auto go = std::make_shared<GameObject>("Background");
		//go->AddComponent(new Texture2DComponent("background.jpg"));
		//scene.Add(go);
		///logo
		//go = std::make_shared<GameObject>("Logo");
		//go->AddComponent(new TransformComponent(glm::vec3{ 216,180,0 }));
		//go->AddComponent(new Texture2DComponent("logo.png"));
		//scene.Add(go);
		////titel
		//go = std::make_shared<GameObject>("Titel");
		//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		//go->AddComponent(new TransformComponent(glm::vec3{ 110,110,0 }));
		//go->AddComponent(new TextComponent("Programming 4 Assignment", font, SDL_Color{ 255,255,255 }));
		//scene.Add(go);
		//fps counter
		go = std::make_shared<GameObject>("FPSCounter");
		auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
		go->AddComponent(new FPSTextComponent(font2));
		scene.Add(go);
		////----------------------------PLAYER ONE-------------------------------
		////Player1
		//auto playerOneIndicator = std::make_shared<GameObject>("Player1");
		//playerOneIndicator->AddComponent(new TransformComponent(glm::vec3(50, 300, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//playerOneIndicator->AddComponent(new TextComponent("Player 1 :", font, SDL_Color{ 255,255,255 }));
		//scene.Add(playerOneIndicator);
		////score
		//auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
		//scoreDisplay->AddComponent(new TransformComponent(glm::vec3(150, 300, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,0 });
		//scoreDisplay->AddComponent(scoreCounter);
		//scene.Add(scoreDisplay);
		////lives
		//auto livesDisplay = std::make_shared<GameObject>("LivesDisplay");
		//livesDisplay->AddComponent(new TransformComponent(glm::vec3(250, 300, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//auto livesCounter = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,0,0 });
		//livesDisplay->AddComponent(livesCounter);
		//scene.Add(livesDisplay);
		////q*bert
		//auto qbert = std::make_shared<GameObject>("Q*Bert");
		//qbert->AddComponent(new TransformComponent(glm::vec3(0, 0, 0)));
		//qbert->AddComponent(new HealthComponent(3));
		//qbert->AddComponent(new ScoreComponent(0));
		//qbert->AddWatcher(new LivesObserver());
		//qbert->AddWatcher(new ScoreObserver());
		//scene.Add(qbert);
		//scene.AddPlayer(qbert);
		////player died text
		//auto playerDied = std::make_shared<GameObject>("Player 1 Died!");
		//playerDied->AddComponent(new TransformComponent(glm::vec3(500, 300, 0)));
		//playerDied->AddComponent(new TextComponent("Player 1 Died!", font, SDL_Color{ 255, 0, 0 }, false));
		//scene.Add(playerDied);
		////----------------------------PLAYER TWO-------------------------------
		////Player2
		//auto playerTwoIndicator = std::make_shared<GameObject>("Player2");
		//playerTwoIndicator->AddComponent(new TransformComponent(glm::vec3(50, 350, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//playerTwoIndicator->AddComponent(new TextComponent("Player 2 :", font, SDL_Color{ 255,255,255 }));
		//scene.Add(playerTwoIndicator);
		////score
		//auto scoreDisplay2 = std::make_shared<GameObject>("ScoreDisplay2");
		//scoreDisplay2->AddComponent(new TransformComponent(glm::vec3(150, 350, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//auto scoreCounter2 = new TextComponent("Score: 0", font, SDL_Color{ 255,255,0 });
		//scoreDisplay2->AddComponent(scoreCounter2);
		//scene.Add(scoreDisplay2);
		////lives
		//auto livesDisplay2 = std::make_shared<GameObject>("LivesDisplay2");
		//livesDisplay2->AddComponent(new TransformComponent(glm::vec3(250, 350, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//auto livesCounter2 = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,0,0 });
		//livesDisplay2->AddComponent(livesCounter2);
		//scene.Add(livesDisplay2);
		////q*bert
		//auto qbert2 = std::make_shared<GameObject>("Q*Bert2");
		//qbert2->AddComponent(new TransformComponent(glm::vec3(0, 0, 0)));
		//qbert2->AddComponent(new HealthComponent(3));
		//qbert2->AddComponent(new ScoreComponent(0));
		//qbert2->AddWatcher(new LivesObserver());
		//qbert2->AddWatcher(new ScoreObserver());
		//scene.Add(qbert2);
		//scene.AddPlayer(qbert2);
		////player 2 died text
		//auto player2Died = std::make_shared<GameObject>("Player 2 Died!");
		//player2Died->AddComponent(new TransformComponent(glm::vec3(500, 350, 0)));
		//player2Died->AddComponent(new TextComponent("Player 2 Died!", font, SDL_Color{ 255,0,0 }, false));
		//scene.Add(player2Died);
		////-----------------------------HOW TO PLAY---------------------------
		//auto instruction = std::make_shared<GameObject>("Instruction");
		//instruction->AddComponent(new TransformComponent(glm::vec3(20, 400, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//instruction->AddComponent(new TextComponent("Player 1: Press A to kill, Press B to add 25 scores.", font, SDL_Color{ 255,165,0 }));
		//scene.Add(instruction);
		////
		//instruction = std::make_shared<GameObject>("InstructionLine2");
		//instruction->AddComponent(new TransformComponent(glm::vec3(20, 420, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//instruction->AddComponent(new TextComponent("Player 2: Press X to kill, Press Y to add 25 scores.", font, SDL_Color{ 255,165,0 }));
		//scene.Add(instruction);
		////
		//instruction = std::make_shared<GameObject>("InstructionLine3");
		//instruction->AddComponent(new TransformComponent(glm::vec3(20, 440, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//instruction->AddComponent(new TextComponent("Press back button to exit.", font, SDL_Color{ 255,165,0 }));
		//scene.Add(instruction);
		////
		//instruction = std::make_shared<GameObject>("Howtoplay");
		//instruction->AddComponent(new TransformComponent(glm::vec3(10, 380, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//instruction->AddComponent(new TextComponent("How to play :", font, SDL_Color{ 255,70,0 }));
		//scene.Add(instruction);
		////q*bert
		//auto qbert3 = std::make_shared<GameObject>("Q*Bert3");
		//qbert3->AddComponent(new TransformComponent(glm::vec3(150, 200, 0)));
		//qbert3->AddComponent(new Texture2DComponent("Coily.png"));
		//qbert3->AddComponent(new AnimationComponent(8));
		//scene.Add(qbert3);
		//scene.AddPlayer(qbert3);
		//---------------------------------------------------------------------------------THE GAME-----------------------------------------
		//Player1
	}
	////------------------------------------------------------------------------------------------------UI
	//player
	//auto playerOne = std::make_shared<GameObject>("Player1");
	//playerOne->AddComponent(new TransformComponent(glm::vec3(50, 50, 0)));
	//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	//playerOne->AddComponent(new TextComponent("Player 1 :", font, SDL_Color{ 255,255,255 }));
	//scene.Add(playerOne);
	//score
	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec2(150, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,0 });
	scoreDisplay->AddComponent(scoreCounter);
	scene.Add(scoreDisplay);
	//lives
	auto livesDisplay = std::make_shared<GameObject>("LivesDisplay");
	livesDisplay->AddComponent(new TransformComponent(glm::vec2(250, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto livesCounter = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,0,0 });
	livesDisplay->AddComponent(livesCounter);
	scene.Add(livesDisplay);
	////------------------------------------------------------------------------------------------------GAME
	//level
	auto level = std::make_shared<GameObject>("Pyramid");
	level->AddComponent(new PyramidComponent(glm::vec2(windowSurface->w / 2.0f, windowSurface->h / 2.0f - 10)));
	scene.Add(level);
	scene.AddLevel(level);
	scene.SetCurrentLevel(level);
	//---------------------------------PLAYER
	//q*bert
	const int playerWidth = 16;
	const int playerHeight = 16;
	auto qbert = std::make_shared<GameObject>("Q*Bert");
	qbert->AddComponent(new TransformComponent(glm::vec2(windowSurface->w / 2 + playerWidth, windowSurface->h / 2 - playerHeight), glm::vec2(playerWidth, playerHeight)));
	qbert->AddComponent(new HealthComponent(3));
	qbert->AddComponent(new ScoreComponent(0));
	qbert->AddWatcher(new LivesObserver());
	qbert->AddWatcher(new ScoreObserver());
	qbert->AddComponent(new Texture2DComponent("QBERT.png", 2, true));
	qbert->AddComponent(new PlayerMovementComponent());
	qbert->AddComponent(new AnimationComponent(8));
	scene.Add(qbert);
	scene.AddPlayer(qbert);
	CollisionDetectionManager::GetInstance().AddCollisionObject(qbert);
	//player died text
	auto playerDied = std::make_shared<GameObject>("Player 1 Died!");
	playerDied->AddComponent(new TransformComponent(glm::vec2(500, 300)));
	playerDied->AddComponent(new TextComponent("Player 1 Died!", font, SDL_Color{ 255, 0, 0 }, false));
	scene.Add(playerDied);
	//---------------------------------ENEMIES
	int enemyWidth = 15;
	int enemyHeight = 16;

	glm::vec2 wrongwaySpawnPos = glm::vec2(464 - enemyWidth * 1.5f, 622 + enemyHeight * 3);
	glm::vec2 uggSpawnPos = glm::vec2(848 + enemyWidth * 1.5f, 622 + enemyHeight * 3);
	glm::vec2 samSpawnPos = glm::vec2(level->GetComponent<PyramidComponent>()->GetSpecificCube(7)->GetPosition().x + enemyWidth, level->GetComponent<PyramidComponent>()->GetSpecificCube(7)->GetPosition().y - enemyHeight);
	glm::vec2 slickSpawnPos = glm::vec2(level->GetComponent<PyramidComponent>()->GetSpecificCube(1)->GetPosition().x + enemyWidth, level->GetComponent<PyramidComponent>()->GetSpecificCube(1)->GetPosition().y - enemyHeight);
	//wrongway
	auto wrongWay = std::make_shared<GameObject>("WrongWay");
	wrongWay->AddComponent(new TransformComponent(wrongwaySpawnPos, glm::vec2(enemyWidth, enemyHeight)));
	wrongWay->AddComponent(new HealthComponent(1));
	wrongWay->AddWatcher(new LivesObserver());
	wrongWay->AddComponent(new Texture2DComponent("WrongWay.png", 2, true));
	wrongWay->AddComponent(new EnemyMovementComponent(qbert, EnemyMovementComponent::EnemyType::WrongWay));
	wrongWay->AddComponent(new AnimationComponent(8));
	scene.Add(wrongWay);
	scene.AddPlayer(wrongWay);
	CollisionDetectionManager::GetInstance().AddCollisionObject(wrongWay);
	//ugg
	auto ugg = std::make_shared<GameObject>("Ugg");
	ugg->AddComponent(new TransformComponent(uggSpawnPos, glm::vec2(enemyWidth, enemyHeight)));
	ugg->AddComponent(new HealthComponent(1));
	ugg->AddWatcher(new LivesObserver());
	ugg->AddComponent(new Texture2DComponent("Ugg.png", 2, true));
	ugg->AddComponent(new EnemyMovementComponent(qbert, EnemyMovementComponent::EnemyType::Ugg));
	ugg->AddComponent(new AnimationComponent(8));
	scene.Add(ugg);
	scene.AddPlayer(ugg);
	CollisionDetectionManager::GetInstance().AddCollisionObject(ugg);
	//sam
	auto sam = std::make_shared<GameObject>("Sam");
	sam->AddComponent(new TransformComponent(samSpawnPos, glm::vec2(enemyWidth, enemyHeight)));
	sam->AddComponent(new HealthComponent(1));
	sam->AddWatcher(new LivesObserver());
	sam->AddComponent(new Texture2DComponent("Sam.png", 2, true));
	sam->AddComponent(new EnemyMovementComponent(qbert, EnemyMovementComponent::EnemyType::Sam));
	sam->AddComponent(new AnimationComponent(8));
	scene.Add(sam);
	scene.AddPlayer(sam);
	CollisionDetectionManager::GetInstance().AddCollisionObject(sam);
	//slick
	auto slick = std::make_shared<GameObject>("Slick");
	slick->AddComponent(new TransformComponent(slickSpawnPos, glm::vec2(enemyWidth, enemyHeight)));
	slick->AddComponent(new HealthComponent(1));
	slick->AddWatcher(new LivesObserver());
	slick->AddComponent(new Texture2DComponent("Slick.png", 2, true));
	slick->AddComponent(new EnemyMovementComponent(qbert, EnemyMovementComponent::EnemyType::Slick));
	slick->AddComponent(new AnimationComponent(8));
	scene.Add(slick);
	scene.AddPlayer(slick);
	CollisionDetectionManager::GetInstance().AddCollisionObject(slick);
}

void dae::Minigin::LoadCoOpScene() const
{
	//auto player = std::make_shared<GameObject>("Player1");
	//
	//const dae::Vector2 playerHalfSize = { 8,8 };
	//const glm::vec2 playerPos = { (m_WindowSurface->w / 2 + playerHalfSize.x) - 96, (m_WindowSurface->h / 2 - playerHalfSize.y) + 144,6 };
	//
	//ControlComponent* controlComponent = new ControlComponent(playerPos);
	//
	//player->AddComponent(controlComponent);
	//SDL_Rect playerSrcRect = { 0,0,16,16 };
	//
	//player->AddComponent(new RenderComponent(playerSrcRect));
	//player->SetTexture("Textures/Qbert2.png");
	////const dae::Vector2& cubeOffset =  mapComp->GetCubeOffset();
	//
	//player->AddComponent(new TransformComponent(playerPos, 1.0f));
	//
	//player->AddComponent(new HealthComponent(3));
	//player->AddComponent(new ScoreComponent(0));
	//
	//player->AddWatcher(new LivesObserver());
	//player->AddWatcher(new ScoreObserver());
	//player->AddComponent(new MoveComponent(27));
	//player->AddTag(dae::Tag::Player);
	//player->AddTag(dae::Tag::Player1);
	//CollisionManager::GetInstance().AddCollider(player);
	//scene.AddPlayer(player);

	//auto player = std::make_shared<GameObject>("Player2");
	//
	//SDL_Rect playerSrcRect = { 0,0,16,16 };
	//const dae::Vector2 playerHalfSize = { 8,8 };
	//player->AddComponent(new RenderComponent(playerSrcRect));
	//player->SetTexture("Textures/Qbert2.png");
	////const dae::Vector2& cubeOffset =  mapComp->GetCubeOffset();
	//const glm::vec2 playerPos = { (m_WindowSurface->w / 2 + playerHalfSize.x) + 96, (m_WindowSurface->h / 2 - playerHalfSize.y) + 144,6 };
	//ControlComponent* controlComponent = new ControlComponent(playerPos);
	//
	//player->AddComponent(controlComponent);
	//
	//player->AddComponent(new TransformComponent(playerPos, 1.0f));
	//
	//player->AddComponent(new HealthComponent(3));
	//player->AddComponent(new ScoreComponent(0));
	//
	//player->AddWatcher(new LivesObserver());
	//player->AddWatcher(new ScoreObserver());
	//player->AddComponent(new MoveComponent(6));
	//player->AddTag(dae::Tag::Player);
	//player->AddTag(dae::Tag::Player2);
	//CollisionManager::GetInstance().AddCollider(player);
	//scene.AddPlayer(player);

	SDL_Surface* windowSurface = SDL_GetWindowSurface(m_Window);
	auto& scene = SceneManager::GetInstance().CreateScene("Qbert");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	{
		///background
		auto go = std::make_shared<GameObject>("Background");
		//go->AddComponent(new Texture2DComponent("background.jpg"));
		//scene.Add(go);
		///logo
		//go = std::make_shared<GameObject>("Logo");
		//go->AddComponent(new TransformComponent(glm::vec3{ 216,180,0 }));
		//go->AddComponent(new Texture2DComponent("logo.png"));
		//scene.Add(go);
		////titel
		//go = std::make_shared<GameObject>("Titel");
		//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		//go->AddComponent(new TransformComponent(glm::vec3{ 110,110,0 }));
		//go->AddComponent(new TextComponent("Programming 4 Assignment", font, SDL_Color{ 255,255,255 }));
		//scene.Add(go);
		//fps counter
		go = std::make_shared<GameObject>("FPSCounter");
		auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
		go->AddComponent(new FPSTextComponent(font2));
		scene.Add(go);
		////----------------------------PLAYER ONE-------------------------------
		////Player1
		//auto playerOneIndicator = std::make_shared<GameObject>("Player1");
		//playerOneIndicator->AddComponent(new TransformComponent(glm::vec3(50, 300, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//playerOneIndicator->AddComponent(new TextComponent("Player 1 :", font, SDL_Color{ 255,255,255 }));
		//scene.Add(playerOneIndicator);
		////score
		//auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
		//scoreDisplay->AddComponent(new TransformComponent(glm::vec3(150, 300, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,0 });
		//scoreDisplay->AddComponent(scoreCounter);
		//scene.Add(scoreDisplay);
		////lives
		//auto livesDisplay = std::make_shared<GameObject>("LivesDisplay");
		//livesDisplay->AddComponent(new TransformComponent(glm::vec3(250, 300, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//auto livesCounter = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,0,0 });
		//livesDisplay->AddComponent(livesCounter);
		//scene.Add(livesDisplay);
		////q*bert
		//auto qbert = std::make_shared<GameObject>("Q*Bert");
		//qbert->AddComponent(new TransformComponent(glm::vec3(0, 0, 0)));
		//qbert->AddComponent(new HealthComponent(3));
		//qbert->AddComponent(new ScoreComponent(0));
		//qbert->AddWatcher(new LivesObserver());
		//qbert->AddWatcher(new ScoreObserver());
		//scene.Add(qbert);
		//scene.AddPlayer(qbert);
		////player died text
		//auto playerDied = std::make_shared<GameObject>("Player 1 Died!");
		//playerDied->AddComponent(new TransformComponent(glm::vec3(500, 300, 0)));
		//playerDied->AddComponent(new TextComponent("Player 1 Died!", font, SDL_Color{ 255, 0, 0 }, false));
		//scene.Add(playerDied);
		////----------------------------PLAYER TWO-------------------------------
		////Player2
		//auto playerTwoIndicator = std::make_shared<GameObject>("Player2");
		//playerTwoIndicator->AddComponent(new TransformComponent(glm::vec3(50, 350, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//playerTwoIndicator->AddComponent(new TextComponent("Player 2 :", font, SDL_Color{ 255,255,255 }));
		//scene.Add(playerTwoIndicator);
		////score
		//auto scoreDisplay2 = std::make_shared<GameObject>("ScoreDisplay2");
		//scoreDisplay2->AddComponent(new TransformComponent(glm::vec3(150, 350, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//auto scoreCounter2 = new TextComponent("Score: 0", font, SDL_Color{ 255,255,0 });
		//scoreDisplay2->AddComponent(scoreCounter2);
		//scene.Add(scoreDisplay2);
		////lives
		//auto livesDisplay2 = std::make_shared<GameObject>("LivesDisplay2");
		//livesDisplay2->AddComponent(new TransformComponent(glm::vec3(250, 350, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//auto livesCounter2 = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,0,0 });
		//livesDisplay2->AddComponent(livesCounter2);
		//scene.Add(livesDisplay2);
		////q*bert
		//auto qbert2 = std::make_shared<GameObject>("Q*Bert2");
		//qbert2->AddComponent(new TransformComponent(glm::vec3(0, 0, 0)));
		//qbert2->AddComponent(new HealthComponent(3));
		//qbert2->AddComponent(new ScoreComponent(0));
		//qbert2->AddWatcher(new LivesObserver());
		//qbert2->AddWatcher(new ScoreObserver());
		//scene.Add(qbert2);
		//scene.AddPlayer(qbert2);
		////player 2 died text
		//auto player2Died = std::make_shared<GameObject>("Player 2 Died!");
		//player2Died->AddComponent(new TransformComponent(glm::vec3(500, 350, 0)));
		//player2Died->AddComponent(new TextComponent("Player 2 Died!", font, SDL_Color{ 255,0,0 }, false));
		//scene.Add(player2Died);
		////-----------------------------HOW TO PLAY---------------------------
		//auto instruction = std::make_shared<GameObject>("Instruction");
		//instruction->AddComponent(new TransformComponent(glm::vec3(20, 400, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//instruction->AddComponent(new TextComponent("Player 1: Press A to kill, Press B to add 25 scores.", font, SDL_Color{ 255,165,0 }));
		//scene.Add(instruction);
		////
		//instruction = std::make_shared<GameObject>("InstructionLine2");
		//instruction->AddComponent(new TransformComponent(glm::vec3(20, 420, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//instruction->AddComponent(new TextComponent("Player 2: Press X to kill, Press Y to add 25 scores.", font, SDL_Color{ 255,165,0 }));
		//scene.Add(instruction);
		////
		//instruction = std::make_shared<GameObject>("InstructionLine3");
		//instruction->AddComponent(new TransformComponent(glm::vec3(20, 440, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//instruction->AddComponent(new TextComponent("Press back button to exit.", font, SDL_Color{ 255,165,0 }));
		//scene.Add(instruction);
		////
		//instruction = std::make_shared<GameObject>("Howtoplay");
		//instruction->AddComponent(new TransformComponent(glm::vec3(10, 380, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//instruction->AddComponent(new TextComponent("How to play :", font, SDL_Color{ 255,70,0 }));
		//scene.Add(instruction);
		////q*bert
		//auto qbert3 = std::make_shared<GameObject>("Q*Bert3");
		//qbert3->AddComponent(new TransformComponent(glm::vec3(150, 200, 0)));
		//qbert3->AddComponent(new Texture2DComponent("Coily.png"));
		//qbert3->AddComponent(new AnimationComponent(8));
		//scene.Add(qbert3);
		//scene.AddPlayer(qbert3);
		//---------------------------------------------------------------------------------THE GAME-----------------------------------------
		//Player1
	}
	////------------------------------------------------------------------------------------------------UI
	//player
	//auto playerOne = std::make_shared<GameObject>("Player1");
	//playerOne->AddComponent(new TransformComponent(glm::vec3(50, 50, 0)));
	//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	//playerOne->AddComponent(new TextComponent("Player 1 :", font, SDL_Color{ 255,255,255 }));
	//scene.Add(playerOne);
	//score
	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec2(150, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,0 });
	scoreDisplay->AddComponent(scoreCounter);
	scene.Add(scoreDisplay);
	//lives
	auto livesDisplay = std::make_shared<GameObject>("LivesDisplay");
	livesDisplay->AddComponent(new TransformComponent(glm::vec2(250, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto livesCounter = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,0,0 });
	livesDisplay->AddComponent(livesCounter);
	scene.Add(livesDisplay);
	////------------------------------------------------------------------------------------------------GAME
	//level
	auto level = std::make_shared<GameObject>("Pyramid");
	level->AddComponent(new PyramidComponent(glm::vec2(windowSurface->w / 2.0f, windowSurface->h / 2.0f - 10)));
	scene.Add(level);
	scene.AddLevel(level);
	scene.SetCurrentLevel(level);
	//---------------------------------PLAYER
	//q*bert
	const int playerWidth = 16;
	const int playerHeight = 16;
	auto qbert = std::make_shared<GameObject>("Q*Bert");
	qbert->AddComponent(new TransformComponent(glm::vec2(windowSurface->w / 2 + playerWidth, windowSurface->h / 2 - playerHeight), glm::vec2(playerWidth, playerHeight)));
	qbert->AddComponent(new HealthComponent(3));
	qbert->AddComponent(new ScoreComponent(0));
	qbert->AddWatcher(new LivesObserver());
	qbert->AddWatcher(new ScoreObserver());
	qbert->AddComponent(new Texture2DComponent("QBERT.png", 2, true));
	qbert->AddComponent(new PlayerMovementComponent());
	qbert->AddComponent(new AnimationComponent(8));
	scene.Add(qbert);
	scene.AddPlayer(qbert);
	CollisionDetectionManager::GetInstance().AddCollisionObject(qbert);
	//player died text
	auto playerDied = std::make_shared<GameObject>("Player 1 Died!");
	playerDied->AddComponent(new TransformComponent(glm::vec2(500, 300)));
	playerDied->AddComponent(new TextComponent("Player 1 Died!", font, SDL_Color{ 255, 0, 0 }, false));
	scene.Add(playerDied);
	//q*bert2
	//q*bert
	auto qbert2 = std::make_shared<GameObject>("Q*Bert2");
	qbert2->AddComponent(new TransformComponent(glm::vec2(windowSurface->w / 2, windowSurface->h / 2), glm::vec2(playerWidth, playerHeight)));
	qbert2->AddComponent(new HealthComponent(3));
	qbert2->AddComponent(new ScoreComponent(0));
	qbert2->AddWatcher(new LivesObserver());
	qbert2->AddWatcher(new ScoreObserver());
	qbert2->AddComponent(new Texture2DComponent("QBERT.png", 2, true));
	qbert2->AddComponent(new PlayerMovementComponent());
	qbert2->AddComponent(new AnimationComponent(8));
	scene.Add(qbert2);
	scene.AddPlayer(qbert2);
	CollisionDetectionManager::GetInstance().AddCollisionObject(qbert2);
	//player died text
	auto playerDied2 = std::make_shared<GameObject>("Player 2 Died!");
	playerDied2->AddComponent(new TransformComponent(glm::vec2(500, 300)));
	playerDied2->AddComponent(new TextComponent("Player 2 Died!", font, SDL_Color{ 255, 0, 0 }, false));
	scene.Add(playerDied2);
}

void dae::Minigin::LoadVersusScene() const
{
	SDL_Surface* windowSurface = SDL_GetWindowSurface(m_Window);
	auto& scene = SceneManager::GetInstance().CreateScene("Qbert");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	{
		///background
		auto go = std::make_shared<GameObject>("Background");
		//go->AddComponent(new Texture2DComponent("background.jpg"));
		//scene.Add(go);
		///logo
		//go = std::make_shared<GameObject>("Logo");
		//go->AddComponent(new TransformComponent(glm::vec3{ 216,180,0 }));
		//go->AddComponent(new Texture2DComponent("logo.png"));
		//scene.Add(go);
		////titel
		//go = std::make_shared<GameObject>("Titel");
		//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		//go->AddComponent(new TransformComponent(glm::vec3{ 110,110,0 }));
		//go->AddComponent(new TextComponent("Programming 4 Assignment", font, SDL_Color{ 255,255,255 }));
		//scene.Add(go);
		//fps counter
		go = std::make_shared<GameObject>("FPSCounter");
		auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
		go->AddComponent(new FPSTextComponent(font2));
		scene.Add(go);
		////----------------------------PLAYER ONE-------------------------------
		////Player1
		//auto playerOneIndicator = std::make_shared<GameObject>("Player1");
		//playerOneIndicator->AddComponent(new TransformComponent(glm::vec3(50, 300, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//playerOneIndicator->AddComponent(new TextComponent("Player 1 :", font, SDL_Color{ 255,255,255 }));
		//scene.Add(playerOneIndicator);
		////score
		//auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
		//scoreDisplay->AddComponent(new TransformComponent(glm::vec3(150, 300, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,0 });
		//scoreDisplay->AddComponent(scoreCounter);
		//scene.Add(scoreDisplay);
		////lives
		//auto livesDisplay = std::make_shared<GameObject>("LivesDisplay");
		//livesDisplay->AddComponent(new TransformComponent(glm::vec3(250, 300, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//auto livesCounter = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,0,0 });
		//livesDisplay->AddComponent(livesCounter);
		//scene.Add(livesDisplay);
		////q*bert
		//auto qbert = std::make_shared<GameObject>("Q*Bert");
		//qbert->AddComponent(new TransformComponent(glm::vec3(0, 0, 0)));
		//qbert->AddComponent(new HealthComponent(3));
		//qbert->AddComponent(new ScoreComponent(0));
		//qbert->AddWatcher(new LivesObserver());
		//qbert->AddWatcher(new ScoreObserver());
		//scene.Add(qbert);
		//scene.AddPlayer(qbert);
		////player died text
		//auto playerDied = std::make_shared<GameObject>("Player 1 Died!");
		//playerDied->AddComponent(new TransformComponent(glm::vec3(500, 300, 0)));
		//playerDied->AddComponent(new TextComponent("Player 1 Died!", font, SDL_Color{ 255, 0, 0 }, false));
		//scene.Add(playerDied);
		////----------------------------PLAYER TWO-------------------------------
		////Player2
		//auto playerTwoIndicator = std::make_shared<GameObject>("Player2");
		//playerTwoIndicator->AddComponent(new TransformComponent(glm::vec3(50, 350, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//playerTwoIndicator->AddComponent(new TextComponent("Player 2 :", font, SDL_Color{ 255,255,255 }));
		//scene.Add(playerTwoIndicator);
		////score
		//auto scoreDisplay2 = std::make_shared<GameObject>("ScoreDisplay2");
		//scoreDisplay2->AddComponent(new TransformComponent(glm::vec3(150, 350, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//auto scoreCounter2 = new TextComponent("Score: 0", font, SDL_Color{ 255,255,0 });
		//scoreDisplay2->AddComponent(scoreCounter2);
		//scene.Add(scoreDisplay2);
		////lives
		//auto livesDisplay2 = std::make_shared<GameObject>("LivesDisplay2");
		//livesDisplay2->AddComponent(new TransformComponent(glm::vec3(250, 350, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//auto livesCounter2 = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,0,0 });
		//livesDisplay2->AddComponent(livesCounter2);
		//scene.Add(livesDisplay2);
		////q*bert
		//auto qbert2 = std::make_shared<GameObject>("Q*Bert2");
		//qbert2->AddComponent(new TransformComponent(glm::vec3(0, 0, 0)));
		//qbert2->AddComponent(new HealthComponent(3));
		//qbert2->AddComponent(new ScoreComponent(0));
		//qbert2->AddWatcher(new LivesObserver());
		//qbert2->AddWatcher(new ScoreObserver());
		//scene.Add(qbert2);
		//scene.AddPlayer(qbert2);
		////player 2 died text
		//auto player2Died = std::make_shared<GameObject>("Player 2 Died!");
		//player2Died->AddComponent(new TransformComponent(glm::vec3(500, 350, 0)));
		//player2Died->AddComponent(new TextComponent("Player 2 Died!", font, SDL_Color{ 255,0,0 }, false));
		//scene.Add(player2Died);
		////-----------------------------HOW TO PLAY---------------------------
		//auto instruction = std::make_shared<GameObject>("Instruction");
		//instruction->AddComponent(new TransformComponent(glm::vec3(20, 400, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//instruction->AddComponent(new TextComponent("Player 1: Press A to kill, Press B to add 25 scores.", font, SDL_Color{ 255,165,0 }));
		//scene.Add(instruction);
		////
		//instruction = std::make_shared<GameObject>("InstructionLine2");
		//instruction->AddComponent(new TransformComponent(glm::vec3(20, 420, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//instruction->AddComponent(new TextComponent("Player 2: Press X to kill, Press Y to add 25 scores.", font, SDL_Color{ 255,165,0 }));
		//scene.Add(instruction);
		////
		//instruction = std::make_shared<GameObject>("InstructionLine3");
		//instruction->AddComponent(new TransformComponent(glm::vec3(20, 440, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//instruction->AddComponent(new TextComponent("Press back button to exit.", font, SDL_Color{ 255,165,0 }));
		//scene.Add(instruction);
		////
		//instruction = std::make_shared<GameObject>("Howtoplay");
		//instruction->AddComponent(new TransformComponent(glm::vec3(10, 380, 0)));
		//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		//instruction->AddComponent(new TextComponent("How to play :", font, SDL_Color{ 255,70,0 }));
		//scene.Add(instruction);
		////q*bert
		//auto qbert3 = std::make_shared<GameObject>("Q*Bert3");
		//qbert3->AddComponent(new TransformComponent(glm::vec3(150, 200, 0)));
		//qbert3->AddComponent(new Texture2DComponent("Coily.png"));
		//qbert3->AddComponent(new AnimationComponent(8));
		//scene.Add(qbert3);
		//scene.AddPlayer(qbert3);
		//---------------------------------------------------------------------------------THE GAME-----------------------------------------
		//Player1
	}
	////------------------------------------------------------------------------------------------------UI
	//player
	//auto playerOne = std::make_shared<GameObject>("Player1");
	//playerOne->AddComponent(new TransformComponent(glm::vec3(50, 50, 0)));
	//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	//playerOne->AddComponent(new TextComponent("Player 1 :", font, SDL_Color{ 255,255,255 }));
	//scene.Add(playerOne);
	//score
	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec2(150, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,0 });
	scoreDisplay->AddComponent(scoreCounter);
	scene.Add(scoreDisplay);
	//lives
	auto livesDisplay = std::make_shared<GameObject>("LivesDisplay");
	livesDisplay->AddComponent(new TransformComponent(glm::vec2(250, 50)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto livesCounter = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,0,0 });
	livesDisplay->AddComponent(livesCounter);
	scene.Add(livesDisplay);
	////------------------------------------------------------------------------------------------------GAME
	//level
	auto level = std::make_shared<GameObject>("Pyramid");
	level->AddComponent(new PyramidComponent(glm::vec2(windowSurface->w / 2.0f, windowSurface->h / 2.0f - 10)));
	scene.Add(level);
	scene.AddLevel(level);
	scene.SetCurrentLevel(level);
	//---------------------------------PLAYER
	//q*bert
	const int playerWidth = 16;
	const int playerHeight = 16;
	auto qbert = std::make_shared<GameObject>("Q*Bert");
	qbert->AddComponent(new TransformComponent(glm::vec2(windowSurface->w / 2 + playerWidth, windowSurface->h / 2 - playerHeight), glm::vec2(playerWidth, playerHeight)));
	qbert->AddComponent(new HealthComponent(3));
	qbert->AddComponent(new ScoreComponent(0));
	qbert->AddWatcher(new LivesObserver());
	qbert->AddWatcher(new ScoreObserver());
	qbert->AddComponent(new Texture2DComponent("QBERT.png", 2, true));
	qbert->AddComponent(new PlayerMovementComponent());
	qbert->AddComponent(new AnimationComponent(8));
	scene.Add(qbert);
	scene.AddPlayer(qbert);
	CollisionDetectionManager::GetInstance().AddCollisionObject(qbert);
	//-----------------------------------COILY VERSUS
	//coily
	int enemyWidth = 16;
	int enemyHeight = 48;
	auto coily = std::make_shared<GameObject>("Coily");
	coily->AddComponent(new TransformComponent(glm::vec2(windowSurface->w / 2 + enemyWidth, windowSurface->h / 2 - enemyHeight), glm::vec2(enemyWidth, enemyHeight)));
	coily->AddComponent(new HealthComponent(1));
	coily->AddWatcher(new LivesObserver());
	coily->AddComponent(new Texture2DComponent("Coily.png", 2, true));
	coily->AddComponent(new PlayerMovementComponent(true));
	coily->AddComponent(new AnimationComponent(8));
	scene.Add(coily);
	scene.AddPlayer(coily);
	CollisionDetectionManager::GetInstance().AddCollisionObject(coily);
	//player died text
	auto playerDied = std::make_shared<GameObject>("Player 1 Died!");
	playerDied->AddComponent(new TransformComponent(glm::vec2(500, 300)));
	playerDied->AddComponent(new TextComponent("Player 1 Died!", font, SDL_Color{ 255, 0, 0 }, false));
	scene.Add(playerDied);
}

void dae::Minigin::LoadGame() const
{
	LoadSinglePlayerScene();
	//LoadCoOpScene();
	//LoadVersusScene();
}