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

using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	//background
	auto go = std::make_shared<GameObject>("Background");
	go->AddComponent(new Texture2DComponent("background.jpg"));
	scene.Add(go);
	//logo
	go = std::make_shared<GameObject>("Logo");
	go->AddComponent(new TransformComponent(glm::vec3{ 216,180,0 }));
	go->AddComponent(new Texture2DComponent("logo.png"));
	scene.Add(go);
	//titel
	go = std::make_shared<GameObject>("Titel");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go->AddComponent(new TransformComponent(glm::vec3{ 110,110,0 }));
	go->AddComponent(new TextComponent("Programming 4 Assignment", font, SDL_Color{ 255,255,255 }));
	scene.Add(go);
	//fps counter
	go = std::make_shared<GameObject>("FPSCounter");
	auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	go->AddComponent(new FPSTextComponent(font2));
	scene.Add(go);
	//----------------------------PLAYER ONE-------------------------------
	//Player1
	auto playerOneIndicator = std::make_shared<GameObject>("Player1");
	playerOneIndicator->AddComponent(new TransformComponent(glm::vec3(50, 300, 0)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	playerOneIndicator->AddComponent(new TextComponent("Player 1 :", font, SDL_Color{ 255,255,255 }));
	scene.Add(playerOneIndicator);
	//score
	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec3(150, 300, 0)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,0 });
	scoreDisplay->AddComponent(scoreCounter);
	scene.Add(scoreDisplay);
	//lives
	auto livesDisplay = std::make_shared<GameObject>("LivesDisplay");
	livesDisplay->AddComponent(new TransformComponent(glm::vec3(250, 300, 0)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto livesCounter = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,0,0 });
	livesDisplay->AddComponent(livesCounter);
	scene.Add(livesDisplay);
	//q*bert
	auto qbert = std::make_shared<GameObject>("Q*Bert");
	qbert->AddComponent(new TransformComponent(glm::vec3(0, 0, 0)));
	qbert->AddComponent(new HealthComponent(3));
	qbert->AddComponent(new ScoreComponent(0));
	qbert->AddWatcher(new LivesObserver());
	qbert->AddWatcher(new ScoreObserver());
	scene.Add(qbert);
	scene.AddPlayer(qbert);
	//player died text
	auto playerDied = std::make_shared<GameObject>("Player 1 Died!");
	playerDied->AddComponent(new TransformComponent(glm::vec3(500, 300, 0)));
	playerDied->AddComponent(new TextComponent("Player 1 Died!", font, SDL_Color{ 255, 0, 0 }, false));
	scene.Add(playerDied);
	//----------------------------PLAYER TWO-------------------------------
	//Player2
	auto playerTwoIndicator = std::make_shared<GameObject>("Player2");
	playerTwoIndicator->AddComponent(new TransformComponent(glm::vec3(50, 350, 0)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	playerTwoIndicator->AddComponent(new TextComponent("Player 2 :", font, SDL_Color{ 255,255,255 }));
	scene.Add(playerTwoIndicator);
	//score
	auto scoreDisplay2 = std::make_shared<GameObject>("ScoreDisplay2");
	scoreDisplay2->AddComponent(new TransformComponent(glm::vec3(150, 350, 0)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scoreCounter2 = new TextComponent("Score: 0", font, SDL_Color{ 255,255,0 });
	scoreDisplay2->AddComponent(scoreCounter2);
	scene.Add(scoreDisplay2);
	//lives
	auto livesDisplay2 = std::make_shared<GameObject>("LivesDisplay2");
	livesDisplay2->AddComponent(new TransformComponent(glm::vec3(250, 350, 0)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto livesCounter2 = new TextComponent("Remaining lives: 3", font, SDL_Color{ 255,0,0 });
	livesDisplay2->AddComponent(livesCounter2);
	scene.Add(livesDisplay2);
	//q*bert
	auto qbert2 = std::make_shared<GameObject>("Q*Bert2");
	qbert2->AddComponent(new TransformComponent(glm::vec3(0, 0, 0)));
	qbert2->AddComponent(new HealthComponent(3));
	qbert2->AddComponent(new ScoreComponent(0));
	qbert2->AddWatcher(new LivesObserver());
	qbert2->AddWatcher(new ScoreObserver());
	scene.Add(qbert2);
	scene.AddPlayer(qbert2);
	//player 2 died text
	auto player2Died = std::make_shared<GameObject>("Player 2 Died!");
	player2Died->AddComponent(new TransformComponent(glm::vec3(500, 350, 0)));
	player2Died->AddComponent(new TextComponent("Player 2 Died!", font, SDL_Color{ 255,0,0 }, false));
	scene.Add(player2Died);
	//-----------------------------HOW TO PLAY---------------------------
	auto instruction = std::make_shared<GameObject>("Instruction");
	instruction->AddComponent(new TransformComponent(glm::vec3(20, 400, 0)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	instruction->AddComponent(new TextComponent("Player 1: Press A to kill, Press B to add 25 scores.", font, SDL_Color{ 255,165,0 }));
	scene.Add(instruction);
	//
	instruction = std::make_shared<GameObject>("InstructionLine2");
	instruction->AddComponent(new TransformComponent(glm::vec3(20, 420, 0)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	instruction->AddComponent(new TextComponent("Player 2: Press X to kill, Press Y to add 25 scores.", font, SDL_Color{ 255,165,0 }));
	scene.Add(instruction);
	//
	instruction = std::make_shared<GameObject>("InstructionLine3");
	instruction->AddComponent(new TransformComponent(glm::vec3(20, 440, 0)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	instruction->AddComponent(new TextComponent("Press back button to exit.", font, SDL_Color{ 255,165,0 }));
	scene.Add(instruction);
	//
	instruction = std::make_shared<GameObject>("Howtoplay");
	instruction->AddComponent(new TransformComponent(glm::vec3(10, 380, 0)));
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	instruction->AddComponent(new TextComponent("How to play :", font, SDL_Color{ 255,70,0 }));
	scene.Add(instruction);
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
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

	input.BindCommands();

	while (doContinue)
	{
		const auto currentTime{ high_resolution_clock::now() };
		const float deltaTime{ duration<float>(currentTime - lastTime).count() };
		lastTime = currentTime;

		input.ProcessInput();
		input.ControllerAnalogs();
		doContinue = input.InputHandler();

		Time::GetInstance().SetDeltaTime(deltaTime);

		sceneManager.Update();
		renderer.Render();
	}

	Cleanup();
}