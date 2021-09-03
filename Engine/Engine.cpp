#include "Engine.h"

namespace ag {

	void Engine::StartUp()
	{
		systems.push_back(std::make_unique<Renderer>());
		systems.push_back(std::make_unique<EventSystem>());
		systems.push_back(std::make_unique<ResourceSystem>());
		systems.push_back(std::make_unique<InputSystem>());
		systems.push_back(std::make_unique<AudioSystem>());
		systems.push_back(std::make_unique<ParticleSystem>());
		systems.push_back(std::make_unique<PhysicsSystem>());

		std::for_each(systems.begin(), systems.end(), [](auto& system) {system->StartUp(); });
		
		REGISTER_CLASS(Actor)
		REGISTER_CLASS(SpriteAnimationComponent)
		REGISTER_CLASS(SpriteComponent)
		REGISTER_CLASS(PhysicsComponent)
		REGISTER_CLASS(RBPhysicsComponent)
		REGISTER_CLASS(AudioComponent)
		REGISTER_CLASS(TextComponent)
	}

	void Engine::ShutDown()
	{
		std::for_each(systems.begin(), systems.end(), [](auto& system) {system->ShutDown(); });
	}

	void Engine::Update()
	{
		time.Tick();
		std::for_each(systems.begin(), systems.end(), [this](auto& system) {system->Update(this->time.deltaTime); });
	}

	void Engine::Draw(Renderer* renderer)
	{
		//

	}
}