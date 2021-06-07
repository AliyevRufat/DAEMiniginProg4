#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "GameStateManager.h"
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
	Locator::GetAudio().AddSound(AudioService::SoundIds::FallEffect, "Resources/Sounds/Fall.wav", true);
	Locator::GetAudio().AddSound(AudioService::SoundIds::JumpEffect, "Resources/Sounds/Jump.wav", true);
	Locator::GetAudio().AddSound(AudioService::SoundIds::LiftEffect, "Resources/Sounds/Lift.wav", true);
	Locator::GetAudio().AddSound(AudioService::SoundIds::VictoryEffect, "Resources/Sounds/Victory.wav", true);
	Locator::GetAudio().AddSound(AudioService::SoundIds::Hurt, "Resources/Sounds/Hurt.wav", true);

	//where second parameter = volume
}

void dae::Minigin::BindCommands()
{
	auto& inputManager = InputManager::GetInstance();

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		//assign buttons
		inputManager.AssignKey<ChooseCoOpGameMode>(ControllerButton::ButtonA, i);
		inputManager.AssignKey<ChooseVersusGameMode>(ControllerButton::ButtonB, i);
		inputManager.AssignKey<ChooseSinglePlayerGameMode>(ControllerButton::ButtonX, i);
		inputManager.AssignKey<GoToMenu>(ControllerButton::ButtonY, i);
		inputManager.AssignKey<ExitCommand>(ControllerButton::ButtonSelect, i);
		//move
		inputManager.AssignKey<JumpUp>(ControllerButton::ButtonUp, i);
		inputManager.AssignKey<JumpDown>(ControllerButton::ButtonDown, i);
		inputManager.AssignKey<JumpLeft>(ControllerButton::ButtonLeft, i);
		inputManager.AssignKey<JumpRight>(ControllerButton::ButtonRight, i);
	}
	//keyboard
	inputManager.AssignKey<JumpUp>(KeyboardButton::W, 0);
	inputManager.AssignKey<JumpDown>(KeyboardButton::S, 0);
	inputManager.AssignKey<JumpLeft>(KeyboardButton::A, 0);
	inputManager.AssignKey<JumpRight>(KeyboardButton::D, 0);

	inputManager.AssignKey<JumpUp>(KeyboardButton::ArrowUp, 1);
	inputManager.AssignKey<JumpDown>(KeyboardButton::ArrowDown, 1);
	inputManager.AssignKey<JumpLeft>(KeyboardButton::ArrowLeft, 1);
	inputManager.AssignKey<JumpRight>(KeyboardButton::ArrowRight, 1);

	inputManager.AssignKey<ChooseCoOpGameMode>(KeyboardButton::O, 0);
	inputManager.AssignKey<ChooseVersusGameMode>(KeyboardButton::P, 0);
	inputManager.AssignKey<ChooseSinglePlayerGameMode>(KeyboardButton::I, 0);
	inputManager.AssignKey<GoToMenu>(KeyboardButton::U, 0);
	inputManager.AssignKey<ExitCommand>(KeyboardButton::ESCAPE, 0);
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
	GameStateManager::GetInstance().SetWindowSurface(SDL_GetWindowSurface(m_Window));

	auto& renderer{ Renderer::GetInstance() };
	auto& sceneManager{ SceneManager::GetInstance() };
	auto& input{ InputManager::GetInstance() };

	bool doContinue{ true };
	auto lastTime{ high_resolution_clock::now() };

	std::thread soundThread(&AudioService::Update, &Locator::GetAudio());

	BindCommands();
	GameStateManager::GetInstance().LoadMenuScreen();

	while (doContinue)
	{
		const auto currentTime{ high_resolution_clock::now() };
		const float deltaTime{ duration<float>(currentTime - lastTime).count() };
		lastTime = currentTime;

		input.ProcessInput();
		input.ControllerAnalogs();
		input.InputHandler();
		doContinue = input.KeyboardInput();
		EngineTime::GetInstance().SetDeltaTime(deltaTime);

		sceneManager.Update();
		renderer.Render();
		EnemyManager::GetInstance().Update();
		CollisionDetectionManager::GetInstance().Update();
	}

	soundThread.detach();

	Cleanup();
}