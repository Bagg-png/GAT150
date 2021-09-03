#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"

int global = 10;

void Game::Initialize()
{
	//Create engine
	engine = std::make_unique<ag::Engine>();
	engine->StartUp();
	engine->Get<ag::Renderer>()->Create("GAME", 800, 600);

	//register classes
	REGISTER_CLASS(ag::PlayerComponent);
	REGISTER_CLASS(ag::EnemyComponent);
	REGISTER_CLASS(ag::PickupComponent);

	//Create scene
	scene = std::make_unique<ag::Scene>();
	scene->engine = engine.get();

	ag::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	ag::SetFilePath("../Resources");

	//events
	engine->Get<ag::EventSystem>()->Subscribe("add_score", std::bind(&Game::OnAddScore, this, std::placeholders::_1));
	engine->Get<ag::EventSystem>()->Subscribe("add_health", std::bind(&Game::OnAddHealth, this, std::placeholders::_1));
	engine->Get<ag::EventSystem>()->Subscribe("sub_health", std::bind(&Game::OnSubHealth, this, std::placeholders::_1));
	engine->Get<ag::EventSystem>()->Subscribe("sub_enemy", std::bind(&Game::OnSubEnemy, this, std::placeholders::_1));
}

void Game::ShutDown()
{
	scene->RemoveAllActors();
	engine->ShutDown();
}

void Game::Update()
{
	engine->Update();

	if (engine->Get<ag::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == ag::eKeyState::PRESSED) {
		quit = true;
	}

	switch (state)
	{
	case Game::eState::Reset:
		Reset();
		break;
	case Game::eState::Title:
		Title();
		break;
	case Game::eState::Instructions:
		Instructions();
		break;
	case Game::eState::StartGame:
		StartGame();
		break;
	case Game::eState::StartLevel:
		StartLevel();
		break;
	case Game::eState::Level:
		Level();
		break;
	case Game::eState::PlayerDead:
		PlayerDead();
		break;
	case Game::eState::GameOver:
		GameOver();
		break;
	default:
		break;
	}

	//update score and health
	auto scoreActor = scene->FindActor("Score");
	if(scoreActor)
	{
	scoreActor->GetComponent<ag::TextComponent>()->SetText("Score: " + std::to_string(score));
	}

	auto healthActor = scene->FindActor("health");
	if (healthActor)
	{
		healthActor->GetComponent<ag::TextComponent>()->SetText("health: " + std::to_string(health));
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<ag::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<ag::Renderer>());
	scene->Draw(engine->Get<ag::Renderer>());

	engine->Get<ag::Renderer>()->EndFrame();
}

void Game::Reset()
{
	scene->RemoveAllActors();

	rapidjson::Document document;
	bool success = ag::json::Load("title.txt", document);
	assert(success);
	scene->Read(document);
	scene->engine->Get<ag::AudioSystem>()->ShutDown();
	scene->engine->Get<ag::AudioSystem>()->StartUp();
	scene->engine->Get<ag::AudioSystem>()->AddAudio("song", "Audio/song.mp3");
	scene->engine->Get<ag::AudioSystem>()->PlayAudio("song", 0.5f, 1, true);
	state = eState::Title;
}

void Game::Title()
{
	scene->FindActor("Title")->active = true;
	scene->FindActor("StartGame")->active = true;
	scene->FindActor("InfoKey")->active = true;
	scene->FindActor("WASD1")->active = false;
	scene->FindActor("WASD2")->active = false;
	scene->FindActor("WASD3")->active = false;
	scene->FindActor("Info")->active = false;
	scene->FindActor("GameOver")->active = false;
	scene->FindActor("Restart")->active = false;

	if (engine->Get<ag::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == ag::eKeyState::PRESSED) {
		auto title = scene->FindActor("Title")->active = false;
		auto startGame = scene->FindActor("StartGame")->active = false;
		auto infoKey = scene->FindActor("InfoKey")->active = false;

		state = eState::StartGame;
	}
	else if (engine->Get<ag::InputSystem>()->GetKeyState(SDL_SCANCODE_I) == ag::eKeyState::PRESSED) {
		state = eState::Instructions;
	}
}

void Game::Instructions()
{
	auto wasd1 = scene->FindActor("WASD1")->active = true;
	auto wasd2 = scene->FindActor("WASD2")->active = true;
	auto wasd3 = scene->FindActor("WASD3")->active = true;
	auto restart = scene->FindActor("Info")->active = true;
	auto title = scene->FindActor("Title")->active = false;
	auto startGame = scene->FindActor("StartGame")->active = false;
	auto infoKey = scene->FindActor("InfoKey")->active = false;
	if (engine->Get<ag::InputSystem>()->GetKeyState(SDL_SCANCODE_I) == ag::eKeyState::PRESSED) {
		state = eState::Title;
	}
}

void Game::StartGame()
{
	rapidjson::Document document;
	bool success = ag::json::Load("scene.txt", document);
	assert(success);
	scene->Read(document);

	ag::Tilemap tilemap;
	tilemap.scene = scene.get();
	success = ag::json::Load("map.txt", document);
	assert(success);
	tilemap.Read(document);
	tilemap.Create();
	SpawnEnemy();

	state = eState::StartLevel;
	stateTimer = 0;
}

void Game::StartLevel()
{
	stateTimer += engine->time.deltaTime;
	if(stateTimer >= 1)
	{
		auto player = ag::ObjectFactory::Instance().Create<ag::Actor>("Player");
		player->transform.position = ag::Vector2{ 400, 200 };
		scene->AddActor(std::move(player));

		coinSpawnTimer = 3;
		healSpawnTimer = 10;
		chestSpawnTimer = 6;
		state = eState::Level;
	}
}

void Game::Level()
{
	coinSpawnTimer -= engine->time.deltaTime;
	healSpawnTimer -= engine->time.deltaTime;
	chestSpawnTimer -= engine->time.deltaTime;
	auto chicken = scene->FindActor("heal");
	auto chest = scene->FindActor("chest");
	if (coinSpawnTimer <= 0) 
	{
		coinSpawnTimer = ag::RandomRange(2, 5);
		auto coin = ag::ObjectFactory::Instance().Create<ag::Actor>("coin");
		coin->transform.position = ag::Vector2{ ag::RandomRange(100, 700), 150.0f };
		scene->AddActor(std::move(coin));
	}
	if (healSpawnTimer <= 0 && !chicken)
	{
		healSpawnTimer = ag::RandomRange(7, 13);
		int pos = ag::RandomRange(1, 6);
		ag::Vector2 randPos = { 0,0 };
		switch (pos)
		{
		case 1:
			randPos = ag::Vector2::pos1;
			break;
		case 2:
			randPos = ag::Vector2::pos2;
			break;
		case 3:
			randPos = ag::Vector2::pos3;
			break;
		case 4:
			randPos = ag::Vector2::pos4;
			break;
		case 5:
			randPos = ag::Vector2::pos5;
			break;
		default:
			break;
		}
		auto heal = ag::ObjectFactory::Instance().Create<ag::Actor>("heal");
		heal->transform.position = randPos;
		scene->AddActor(std::move(heal));
	}
	if (chestSpawnTimer <= 0 && !chest)
	{
		chestSpawnTimer = ag::RandomRange(7, 15);
		int pos = ag::RandomRange(6, 9);
		ag::Vector2 randPos = { 0,0 };
		switch (pos)
		{
		case 6:
			randPos = ag::Vector2::pos6;
			break;
		case 7:
			randPos = ag::Vector2::pos7;
			break;
		case 8:
			randPos = ag::Vector2::pos8;
			break;
		default:
			break;
		}
		auto chest = ag::ObjectFactory::Instance().Create<ag::Actor>("chest");
		chest->transform.position = randPos;
		scene->AddActor(std::move(chest));
	}

}

void Game::PlayerDead()
{
	scene->FindActor("Player")->GetComponent<ag::PlayerComponent>()->state = ag::PlayerComponent::pState::DEAD;

	auto gameOver = scene->FindActor("GameOver");
	assert(gameOver);
	gameOver->active = true;
	auto restart = scene->FindActor("Restart");
	assert(restart);
	restart->active = true;
	if (engine->Get<ag::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == ag::eKeyState::PRESSED) {
		state = eState::Reset;
	}
}

void Game::GameOver()
{

}

void Game::SpawnEnemy()
{
	for (int i = 0; i < enemyCount; i++) {
	auto enemy = ag::ObjectFactory::Instance().Create<ag::Actor>("enemy");
	enemy->transform.position = ag::Vector2{ ag::RandomRange(0,700), ag::RandomRange(0,30) };
	scene->AddActor(std::move(enemy));
	}
}

void Game::OnAddScore(const ag::Event& event)
{
	//grabs score amount from event
	score += std::get<int>(event.data);
}

void Game::OnAddHealth(const ag::Event& event)
{
	health += std::get<int>(event.data);
	if (health > 5) health = 5;
	
}

void Game::OnSubHealth(const ag::Event& event)
{
	health -= std::get<int>(event.data);
	if (health <= 0) {
		health = 0;
		state = eState::PlayerDead;
	}
}

void Game::OnSubEnemy(const ag::Event& event)
{
	enemyCount -= std::get<int>(event.data);
	score += 5;
	if (enemyCount <= 0) {
		enemyStorage++;
		enemyCount = enemyStorage;
		SpawnEnemy();
	}
}