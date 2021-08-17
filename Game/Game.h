#pragma once
#include "../Engine/Engine.h"

class Game {
	public:
		void Initialize();
		void ShutDown();

		void Update();
		void Draw();

		bool IsQuit() { return quit; }

	public:
		std::unique_ptr<ag::Engine> engine;
		std::unique_ptr<ag::Scene> scene;

	private:
		bool quit = false;

		/*ag::AudioChannel musicChannel;
		std::shared_ptr<ag::Texture> particleTexture;
		std::shared_ptr<ag::Texture> playerTexture;
		std::shared_ptr<ag::Texture> enemyTexture;
		std::shared_ptr<ag::Texture> bulletTexture;
		std::shared_ptr<ag::Texture> titleTexture;
		std::shared_ptr<ag::Texture> controlsTexture;
		std::shared_ptr<ag::Texture> overTexture;
		std::shared_ptr<ag::Texture> restartTexture;
		std::shared_ptr<ag::Texture> startTexture;*/
};
