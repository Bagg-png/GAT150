#pragma once
#include "../Engine/Component/Component.h"
#include "Framework/EventSystem.h"

namespace ag {
	class PickupComponent : public Component {
	public:
		std::unique_ptr<Object> Clone() const { return std::make_unique<PickupComponent>(*this); }

		virtual ~PickupComponent();

		void Create() override;
		virtual void Update() override;

		void OnCollisionEnter(const ag::Event& event);

		virtual bool Read(const rapidjson::Value& value) override;
		virtual bool Write(const rapidjson::Value& value)const override;

		//float speed{ 0 };

	};
}
