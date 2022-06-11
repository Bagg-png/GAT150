#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"

namespace ag {
	class PlayerComponent : public Component {
	public:
		enum class pState {
			IDLE,
			WALK_RIGHT,
			WALK_LEFT,
			DASH_RIGHT,
			DASH_LEFT,
			DEAD
		};

		std::unique_ptr<Object> Clone() const { return std::make_unique<PlayerComponent>(*this); }

		virtual ~PlayerComponent();

		void Create() override;
		virtual void Update() override;

		virtual void OnCollisionEnter(const ag::Event& event);
		virtual void OnCollisionExit(const ag::Event& event);

		virtual bool Write(const rapidjson::Value& value)const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		float speed{ 0 };
		float jump{ 0 };
		float dashTimer{ 0 };
		float attackTimer{ 0 };
		float healthTimer{ 0 };
		int health{ 3 };
		pState state = pState::IDLE;

	private:
		std::list<ag::Actor*> contacts;
	};
}

