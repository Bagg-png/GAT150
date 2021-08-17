#include "Scene.h"
#include "Actor.h"
#include <algorithm>

namespace ag {
	void Scene::Update(float dt)
	{
		// Add actors
		actors.insert(actors.end(), std::make_move_iterator(newActors.begin()), std::make_move_iterator(newActors.end()));
		newActors.clear();

		// Update actors
		std::for_each(actors.begin(), actors.end(), [dt](auto& actor) {actor->Update(dt); });
		for (auto& actor : actors) {
			actor->Update(dt);
		}

		// Check collisions
		for (size_t i = 0; i < actors.size(); i++) {

			for (size_t j = i + 1; j < actors.size(); j++) {

				if (actors[i]->destroy || actors[j]->destroy) continue;

				ag::Vector2 dir = actors[i]->transform.position - actors[j]->transform.position;
				float distance = dir.Length();

				if (distance < actors[i]->GetRadius() + actors[j]->GetRadius()) {

					actors[i]->OnCollision(actors[j].get());
					actors[j]->OnCollision(actors[i].get());
				}
			}
		}

		// destroy actor
		auto iter = actors.begin();
		while (iter != actors.end()) {
			if ((*iter)->destroy) {
				iter = actors.erase(iter);
			}
			else {
				iter++;
			}
		}
	}
	void Scene::Draw(Renderer* renderer)
	{
		std::for_each(actors.begin(), actors.end(), [renderer](auto& actor) {actor->Draw(renderer); });
	}
	void Scene::AddActor(std::unique_ptr<Actor> actor)
	{
		actor->scene = this;
		actor->Initialize();

		newActors.push_back(std::move(actor));
	}

	void Scene::RemoveActor(Actor* actor)
	{
	}

	void Scene::RemoveAllActors()
	{
		actors.clear();
	}

}