#include "PickupComponent.h"
#include "../Engine/Engine.h"

using namespace ag;

void ag::PickupComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PickupComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<AudioSystem>()->AddAudio("heal", "Audio/heal.mp3");
	owner->scene->engine->Get<AudioSystem>()->AddAudio("coin", "Audio/coin.wav");
}

void PickupComponent::Update()
{
	/*Actor* player = owner->scene->FindActor("Player");
	if (player)
	{
		Vector2 direction = player->transform.position - owner->transform.position;
		Vector2 force = direction.Normalized() * speed;

		PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
		assert(physicsComponent);

		physicsComponent->ApplyForce(force);
	}*/
}

void PickupComponent::OnCollisionEnter(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "player"))
	{
		if (istring_compare(owner->name, "heal")) {
			owner->destroy = true;
			owner->scene->engine->Get<AudioSystem>()->PlayAudio("heal");

			Event event;
			event.name = "add_health";
			event.data = 1;
			owner->scene->engine->Get<EventSystem>()->Notify(event);
		}
		else {
		owner->destroy = true;
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("coin");

		Event event;
		event.name = "add_score";
		if (istring_compare(owner->name, "chest")) {

		event.data = 30;
		}
		else {

		event.data = 10;
		}
		owner->scene->engine->Get<EventSystem>()->Notify(event);
		}
	}

}

bool PickupComponent::Read(const rapidjson::Value& value)
{
	//JSON_READ(value, speed);

	return true;
}

bool PickupComponent::Write(const rapidjson::Value& value) const
{
	return false;
}
PickupComponent::~PickupComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}