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

	ag::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	ag::SetFilePath("../Resources");

	std::unique_ptr<ag::Actor> actor = std::make_unique <ag::Actor>(ag::Transform{ ag::Vector2{400,300}, 0, 5 });
	{
	std::unique_ptr<ag::SpriteComponent> component = std::make_unique<ag::SpriteComponent>();
	component->texture = engine->Get<ag::ResourceSystem>()->Get<ag::Texture>("player1.png", engine->Get<ag::Renderer>());
	actor->AddComponent(std::move(component));

	}
	{
		std::unique_ptr<ag::PhysicsComponent> component = std::make_unique<ag::PhysicsComponent>();
		component->ApplyForce(ag::Vector2::right * 200);
	actor->AddComponent(std::move(component));
	}
	scene->AddActor(std::move(actor));
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

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<ag::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<ag::Renderer>());
	scene->Draw(engine->Get<ag::Renderer>());

	engine->Get<ag::Renderer>()->EndFrame();
}
