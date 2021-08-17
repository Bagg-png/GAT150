#pragma once
#include "Object/Object.h"

namespace ag {
	class Actor;
	class Component : public Object {
	public:
		virtual void Update() = 0;
	public:
		Actor* owner{ nullptr };
	};
}