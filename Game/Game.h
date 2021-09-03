#pragma once
#include "../Engine/Engine.h"

class Game {
	public:
		enum class eState {
			Reset,
			Title,
			Instructions,
			StartGame,
			StartLevel,
			Level,
			PlayerDead,
			GameOver
		};
		void Initialize();
		void ShutDown();

		void Update();
		void Draw();

		bool IsQuit() { return quit; }

private:
		void Reset();
		void Title();
		void Instructions();
		void StartGame();
		void StartLevel();
		void Level();
		void PlayerDead();
		void GameOver();
		void SpawnEnemy();

		void OnAddScore(const ag::Event& event);
		void OnAddHealth(const ag::Event& event);
		void OnSubHealth(const ag::Event& event);
		void OnSubEnemy(const ag::Event& event);

	public:
		std::unique_ptr<ag::Engine> engine;
		std::unique_ptr<ag::Scene> scene;

	private:
		bool quit = false;
		eState state = eState::Reset;
		float health{ 3 };
		int enemyCount{ 2 };
		int enemyStorage{ 2 };
		int score{ 0 };
		float stateTimer{ 0 };
		float coinSpawnTimer{ 0 };
		float chestSpawnTimer{ 0 };
		float healSpawnTimer{ 0 };
};
