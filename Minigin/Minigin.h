#pragma once
struct SDL_Window;

class ConsoleAudioService;
namespace dae
{
	class Minigin
	{
	public:
		void Run();
	private:
		void Cleanup();
		void Initialize();
		void BindCommands();

		static const int m_MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		const float m_MsPerUpdate = m_MsPerFrame / 1000.0f; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};
	};
}