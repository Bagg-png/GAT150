#include "ParticleSystem.h"
#include "Renderer.h"
#include "../Math/Random.h"

namespace ag {

	void ParticleSystem::StartUp()
	{
		particles.resize(1000);
	}

	void ParticleSystem::ShutDown()
	{
		particles.clear();
	}

	void ParticleSystem::Update(float dt)
	{
		for (Particle& particle : particles) {
			if (particle.isActive) {
				particle.lifetime -= dt;
				particle.isActive = particle.lifetime > 0;
				particle.prevPosition = particle.position;
				particle.position += particle.velocity * dt;
			}
		}
	}

	void ParticleSystem::Draw(Renderer* renderer)
	{
		for (const Particle& particle : particles) {
			if (particle.isActive) {

				//std::for_each(particles.begin(), particles.end(), [&renderer](auto& system) {if (dynamic_cast<GraphicsSystem*>(system.get())) dynamic_cast<GraphicsSystem*>(system.get()); });
			}
			}
	}

	void ParticleSystem::Create(const Vector2& position, size_t count, float lifetime, std::shared_ptr<Texture> texture, float speed)
	{
		for (size_t i = 0; i < count; i++) {
			auto particle = std::find_if(particles.begin(), particles.end(), Particle::IsNotActive);
			if (particle != particles.end()) {
				particle-> isActive = true;
				particle-> lifetime = lifetime;
				particle-> position = position;
				particle-> prevPosition = position;
				particle-> texture = texture;
				//particle-> color = color;

				particle-> velocity = Vector2{ RandomRange(-1,1), RandomRange(-1,1) } * (speed * Random());
			}
		}
	}
	void ParticleSystem::Create(const Vector2& position, size_t count, float lifetime, std::shared_ptr<Texture> texture, float speed, float angle, float angleRange)
	{
		for (size_t i = 0; i < count; i++) {
			auto particle = std::find_if(particles.begin(), particles.end(), Particle::IsNotActive);
			if (particle != particles.end()) {
				particle->isActive = true;
				particle->lifetime = lifetime;
				particle->position = position;
				particle->prevPosition = position;
				particle->texture = texture;
				//particle->color = colors[rand() % colors.size()];

				particle->velocity = ag::Vector2::Rotate( ag::Vector2::right, angle + ag::RandomRange(-angleRange, angleRange)) * (speed * Random());
			}
		}
	}
}