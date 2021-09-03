#include "PlayerComponent.h"
#include "../Engine/Engine.h"
#include "SDL.h"

using namespace ag;

void ag::PlayerComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), owner);
	
	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "Audio/hurt.wav");
	owner->scene->engine->Get<AudioSystem>()->AddAudio("bat", "Audio/bat.mp3");
	owner->scene->engine->Get<AudioSystem>()->AddAudio("dash", "Audio/dash.wav");
}

void PlayerComponent::Update()
{
	if (state != pState::DEAD) {

		Vector2 force = Vector2::zero;
		float now = owner->scene->engine->time.time - dashTimer;

		if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_A) == eKeyState::HELD)
		{
			state = pState::WALK_LEFT;
			force.x -= speed;
		}
		if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_D) == eKeyState::HELD)
		{
			state = pState::WALK_RIGHT;
			force.x += speed;
		}
		if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == eKeyState::PRESSED && now > 2)
		{

			if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_D) == eKeyState::HELD) {
				attackTimer = owner->scene->engine->time.time;
				dashTimer = owner->scene->engine->time.time;
				force.x += 300;
				owner->scene->engine->Get<AudioSystem>()->PlayAudio("dash");
				state = pState::DASH_RIGHT;
			}
			else if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_A) == eKeyState::HELD) {
				attackTimer = owner->scene->engine->time.time;
				dashTimer = owner->scene->engine->time.time;
				force.x -= 300;
				owner->scene->engine->Get<AudioSystem>()->PlayAudio("dash");
				state = pState::DASH_LEFT;
			}
		}
		if (contacts.size() > 0 && owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_W) == eKeyState::PRESSED)
		{
			force.y -= jump;
			state = pState::IDLE;
		}

		PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
		assert(physicsComponent);
		physicsComponent->ApplyForce(force);

		SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
		assert(spriteAnimationComponent);
		if (now > 0 && now < 0.6f)
		{
			if (physicsComponent->velocity.x > 0)
			{
				state = pState::DASH_RIGHT;
			}
			else if (physicsComponent->velocity.x < 0) {

				state = pState::DASH_LEFT;
			}
		}
		else if (physicsComponent->velocity.x > -0.3f && physicsComponent->velocity.x < 0.3f) {
			state = pState::IDLE;
		}
		
		switch (state)
		{
		case ag::PlayerComponent::pState::IDLE:
			spriteAnimationComponent->SetSequence("idle");
			break;
		case ag::PlayerComponent::pState::WALK_RIGHT:
			spriteAnimationComponent->SetSequence("walk_right");
			break;
		case ag::PlayerComponent::pState::WALK_LEFT:
			spriteAnimationComponent->SetSequence("walk_left");
			break;
		case ag::PlayerComponent::pState::DASH_RIGHT:
			spriteAnimationComponent->SetSequence("dash_right");
			break;
		case ag::PlayerComponent::pState::DASH_LEFT:
			spriteAnimationComponent->SetSequence("dash_left");
			break;
		default:
			break;
		}
	}
	else {
		SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
		assert(spriteAnimationComponent);
		spriteAnimationComponent->SetSequence("dead");
	}
}

void ag::PlayerComponent::OnCollisionEnter(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);
	
	float now = owner->scene->engine->time.time - attackTimer;
	float healthNow = owner->scene->engine->time.time - healthTimer;

	//adds to contact list
	if(istring_compare(actor->tag, "ground"))
	{
		contacts.push_back(actor);
	}
	else if (istring_compare(actor->tag, "enemy") && (now > 0 && now < 0.6f))
	{
		Event event;
		event.name = "sub_enemy";
		event.data = 1;
		owner->scene->engine->Get<EventSystem>()->Notify(event);
 		actor->destroy = true;
		//owner->scene->engine->time.start = 0;
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("bat");
	}
	else if(istring_compare(actor->tag, "enemy") && healthNow > 1.5f)
	{
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt");
		Event event;
		event.name = "sub_health";
		event.data = 1;
		owner->scene->engine->Get<EventSystem>()->Notify(event);
		healthTimer = owner->scene->engine->time.time;
	}

	//std::cout << "Just Entered: " << actor->tag << std::endl;
}

void ag::PlayerComponent::OnCollisionExit(const Event& event)
{

	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	//removes from contact list
	if (istring_compare(actor->tag, "ground"))
	{
		contacts.remove(actor);
	}
	//std::cout << "Just Exited: " << actor->tag << std::endl;

}

bool PlayerComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	JSON_READ(value, jump);

	return true;
}

bool PlayerComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

PlayerComponent::~PlayerComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}


