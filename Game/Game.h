#pragma once
#include "../Engine/Engine.h"

class Game {
	public:
		enum class eState {
			Title,
			StartGame,
			StartLevel,
			Game,
			Damage,
			GameOver
		};
	public:
		void Initialize();
		void ShutDown();

		void Update();
		void Draw();

		bool IsQuit() { return quit; }

private:
	void UpdateTitle(float dt);
	void StartGame(float dt);
	void UpdateLevel(float dt);
	void OnAddPoints(const ag::Event& event);
	void OnPlayerDead(const ag::Event& event);

	public:
		std::unique_ptr<ag::Engine> engine;
		std::unique_ptr<ag::Scene> scene;

	private:
		bool quit = false;
		eState state = eState::Title;
		float stateTimer = 0.0f;

		size_t score = 0;
		size_t lives = 0;

		ag::AudioChannel musicChannel;
		std::shared_ptr<ag::Texture> particleTexture;
		std::shared_ptr<ag::Texture> playerTexture;
		std::shared_ptr<ag::Texture> enemyTexture;
		std::shared_ptr<ag::Texture> bulletTexture;
		std::shared_ptr<ag::Texture> titleTexture;
		std::shared_ptr<ag::Texture> controlsTexture;
		std::shared_ptr<ag::Texture> overTexture;
		std::shared_ptr<ag::Texture> restartTexture;
		std::shared_ptr<ag::Texture> startTexture;
};
