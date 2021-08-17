#include "Game.h"
//#include "Actors/Player.h"
//#include "Actors/Enemy.h"
//#include "Actors/Projectile.h"
//#include "Actors/Upgrade.h"

void Game::Initialize()
{
	//Create engine
	engine = std::make_unique<ag::Engine>();
	engine->StartUp();
	engine->Get<ag::Renderer>()->Create("GAME", 800, 600);

	//Create scene
	scene = std::make_unique<ag::Scene>();
	scene->engine = engine.get();
	ag::SetFilePath("../Resources");

	engine->Get<ag::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<ag::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));


	int size = 20;
	std::shared_ptr<ag::Font> font2d = engine->Get<ag::ResourceSystem>()->Get<ag::Font>("Fonts/2dumb.ttf", &size);
	std::shared_ptr<ag::Font> font3d = engine->Get<ag::ResourceSystem>()->Get<ag::Font>("Fonts/3dumb.ttf", &size);

	titleTexture = std::make_shared<ag::Texture>(engine->Get<ag::Renderer>());
	titleTexture->Create(font3d->CreateSurface("DaGame", ag::Color{ 1,1,1 }));
	engine->Get<ag::ResourceSystem>()->Add("titleTexture", titleTexture);
	size = 16;
	controlsTexture = std::make_shared<ag::Texture>(engine->Get<ag::Renderer>());
	controlsTexture->Create(font2d->CreateSurface("Press 'S' Key to cycle colors", ag::Color{ 1,1,1 }));
	engine->Get<ag::ResourceSystem>()->Add("controlsTexture", controlsTexture);

	startTexture = std::make_shared<ag::Texture>(engine->Get<ag::Renderer>());
	startTexture->Create(font2d->CreateSurface("Press Spacebar to Start", ag::Color{ 1,1,1 }));
	engine->Get<ag::ResourceSystem>()->Add("startTexture", startTexture);

	overTexture = std::make_shared<ag::Texture>(engine->Get<ag::Renderer>());
	overTexture->Create(font2d->CreateSurface("GameOver", ag::Color{ 1,0,0 }));
	engine->Get<ag::ResourceSystem>()->Add("overTexture", overTexture);

	restartTexture = std::make_shared<ag::Texture>(engine->Get<ag::Renderer>());
	restartTexture->Create(font2d->CreateSurface("Press Spacebar to Restart", ag::Color{ 1,0,0 }));
	engine->Get<ag::ResourceSystem>()->Add("restartTexture", restartTexture);

	playerTexture = std::make_shared<ag::Texture>(engine->Get<ag::Renderer>());
	bulletTexture = std::make_shared<ag::Texture>(engine->Get<ag::Renderer>());
	enemyTexture = std::make_shared<ag::Texture>(engine->Get<ag::Renderer>());
	
	engine->Get<ag::ResourceSystem>()->Add("playerTexture", playerTexture);
	engine->Get<ag::ResourceSystem>()->Add("bulletTexture", bulletTexture);
	engine->Get<ag::ResourceSystem>()->Add("enemyTexture", enemyTexture);

	engine->Get<ag::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine->Get<ag::AudioSystem>()->AddAudio("music", "audio/song.mp3");
}

void Game::ShutDown()
{
	scene->RemoveAllActors();
	engine->ShutDown();
}

void Game::Update()
{
	engine->Update();
	float dt = engine->time.deltaTime;
	stateTimer += dt;
	//switch tab tab *variable name* enter

	switch (state)
	{
	case Game::eState::Title:
		if (engine->Get<ag::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == ag::eKeyState::PRESSED) {
			state = eState::StartGame;
		}
		break;
	case Game::eState::StartGame:
		//engine->Get<ag::AudioSystem>()->ShutDown();
		engine->Get<ag::AudioSystem>()->StartUp();
		engine->Get<ag::AudioSystem>()->AddAudio("explosion", "Audio/explosion.wav");
		engine->Get<ag::AudioSystem>()->AddAudio("playerFire", "Audio/projectileFire.wav");
		engine->Get<ag::AudioSystem>()->AddAudio("song", "Audio/song.mp3");
		score = 0;
		lives = 3;
		state = eState::StartLevel;
		break;
	case Game::eState::StartLevel:
		StartGame(dt);
		state = eState::Game;
		break;
	case Game::eState::Game:
	musicChannel = engine->Get<ag::AudioSystem>()->PlayAudio("song", 1, 1, true);
		//engine->Get<ag::AudioSystem>()->PlayAudio("song");
		if (scene->GetAllActors<ag::Enemy>().size() == 0) {
			musicChannel.Stop();
			UpdateLevel(dt);
			scene->AddActor(std::make_unique<ag::Upgrade>(ag::Transform{ ag::Vector2{400.0f, 300.0f}, 0.0f, 3.0f }, engine->Get<ag::ResourceSystem>()->Get<ag::Texture>("upgrade.png", engine->Get<ag::Renderer>()),100.0f));
		}
		break;
	case Game::eState::Damage:
		lives -= 1;
		break;
	case Game::eState::GameOver:
		if (engine->Get<ag::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == ag::eKeyState::PRESSED) {
			scene->RemoveAllActors();
			state = eState::StartGame;
		}
		break;
	default:
		break;
	}


	if (engine->Get<ag::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == ag::eKeyState::PRESSED) {
		quit = true;
	}
	//if (engine->Get<ag::InputSystem>()->GetButtonState((int)ag::eMouseButton::RIGHT) == ag::eKeyState::PRESSED) {
	//	ag::Vector2 position = engine->Get<ag::InputSystem>()->GetMousePosition();
	//	//std::cout << position.x << " " << position.y << std::endl;

	//	engine->Get<ag::AudioSystem>()->PlayAudio("explosion", 1, ag::RandomRange(0.2f, 2.0f));
	//	//musicChannel.SetPitch(ag::RandomRange(0.2f, 2.0f));
	//}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<ag::Renderer>()->BeginFrame();
		ag::Transform p;
		p.position = { 350,400 };
	switch (state)
	{
	case Game::eState::Title:
		engine->Get<ag::Renderer>()->Draw(titleTexture, p);
		engine->Get<ag::Renderer>()->Draw(controlsTexture, p.position.y + 30);
		engine->Get<ag::Renderer>()->Draw(startTexture, p.position.y + 60);
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::GameOver:
		engine->Get<ag::Renderer>()->Draw(overTexture, p);
		engine->Get<ag::Renderer>()->Draw(restartTexture, p.position.y + 30);
		break;
	default:
		break;
	}
	/*graphics.SetColor(ag::Color::white);
	graphics.DrawString(30, 20, std::to_string(score).c_str());
	graphics.DrawString(750, 20, std::to_string(lives).c_str());*/


	engine->Draw(engine->Get<ag::Renderer>());
	scene->Draw(engine->Get<ag::Renderer>());

	engine->Get<ag::Renderer>()->EndFrame();
}

void Game::UpdateTitle(float dt)
{
	if (engine->Get<ag::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == ag::eKeyState::PRESSED) {
		state = eState::StartGame;
	}
}

void Game::StartGame(float dt)
{
	int enemyCount = ag::RandomRangeInt(3, 7);
	std::unique_ptr<ag::Player> player = std::make_unique<ag::Player>(ag::Transform{ ag::Vector2{400.0f, 300.0f}, 0.0f, 1.5f },
		engine->Get<ag::ResourceSystem>()->Get<ag::Texture>("player1.png", engine->Get<ag::Renderer>()),
		engine->Get<ag::ResourceSystem>()->Get<ag::Texture>("bulletR2.png", engine->Get<ag::Renderer>()),
		engine->Get<ag::ResourceSystem>()->Get<ag::Texture>("bulletB2.png", engine->Get<ag::Renderer>()),
		engine->Get<ag::ResourceSystem>()->Get<ag::Texture>("bulletY2.png", engine->Get<ag::Renderer>()),
		80.0f);
	scene->AddActor(std::move(player));
		for (size_t i = 0; i < enemyCount; i++) {
			int r = ag::RandomRangeInt(1, 4);
			switch (r) {
			case 1:
				scene->AddActor(std::make_unique<ag::Enemy>(ag::Transform{ ag::Vector2{ag::RandomRange(0.0f,800.0f), ag::RandomRange(0.0f,600.0f)},
					ag::RandomRange(0, ag::TwoPi), ag::RandomRange(1.5f,2.0f) }, engine->Get<ag::ResourceSystem>()->Get<ag::Texture>("enemyR2.png", engine->Get<ag::Renderer>()), "red", ag::RandomRange(30.0f, 60.0f)));
				break;
			case 2:
				scene->AddActor(std::make_unique<ag::Enemy>(ag::Transform{ ag::Vector2{ag::RandomRange(0.0f,800.0f), ag::RandomRange(0.0f,600.0f)},
					ag::RandomRange(0, ag::TwoPi), ag::RandomRange(1.5f,2.0f) }, engine->Get<ag::ResourceSystem>()->Get<ag::Texture>("enemyB2.png", engine->Get<ag::Renderer>()), "blue", ag::RandomRange(30.0f, 60.0f)));
				break;
			case 3:
				scene->AddActor(std::make_unique<ag::Enemy>(ag::Transform{ ag::Vector2{ag::RandomRange(0.0f,800.0f), ag::RandomRange(0.0f,600.0f)},
					ag::RandomRange(0, ag::TwoPi), ag::RandomRange(1.5f,2.0f) }, engine->Get<ag::ResourceSystem>()->Get<ag::Texture>("enemyY2.png", engine->Get<ag::Renderer>()), "yellow", ag::RandomRange(30.0f, 60.0f)));
				break;
			}
		}
}

void Game::UpdateLevel(float dt)
{
	for (size_t i = 0; i < 5; i++) {
		int r = ag::RandomRangeInt(1, 4);
		switch (r) {
		case 1:
			scene->AddActor(std::make_unique<ag::Enemy>(ag::Transform{ ag::Vector2{ag::RandomRange(0.0f,800.0f), ag::RandomRange(0.0f,600.0f)},
					ag::RandomRange(0, ag::TwoPi), ag::RandomRange(2.0f,4.0f) }, engine->Get<ag::ResourceSystem>()->Get<ag::Texture>("enemyR2.png", engine->Get<ag::Renderer>()), "red", ag::RandomRange(30.0f, 60.0f)));
			break;
		case 2:
			scene->AddActor(std::make_unique<ag::Enemy>(ag::Transform{ ag::Vector2{ag::RandomRange(0.0f,800.0f), ag::RandomRange(0.0f,600.0f)},
					ag::RandomRange(0, ag::TwoPi), ag::RandomRange(2.0f,4.0f) }, engine->Get<ag::ResourceSystem>()->Get<ag::Texture>("enemyB2.png", engine->Get<ag::Renderer>()), "blue", ag::RandomRange(30.0f, 60.0f)));
			break;
		case 3:
			scene->AddActor(std::make_unique<ag::Enemy>(ag::Transform{ ag::Vector2{ag::RandomRange(0.0f,800.0f), ag::RandomRange(0.0f,600.0f)},
					ag::RandomRange(0, ag::TwoPi), ag::RandomRange(2.0f,4.0f) }, engine->Get<ag::ResourceSystem>()->Get<ag::Texture>("enemyY2.png", engine->Get<ag::Renderer>()), "yellow", ag::RandomRange(30.0f, 60.0f)));
			break;
		}
	}
}

void Game::OnAddPoints(const ag::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnPlayerDead(const ag::Event& event)
{
	lives--;
	std::cout << std::get<std::string>(event.data) << std::endl;
	state = eState::GameOver;
}
