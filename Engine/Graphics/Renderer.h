#pragma once
#include "Framework/System.h"
#include "Math/Vector2.h"
#include "Math/Transform.h"
#include "Math/MathUtils.h"
#include "Texture.h"
#include <SDL.h>
#include <string>

namespace ag {
	class Renderer : public System {
	public:
		void StartUp() override;
		void ShutDown() override;
		void Update(float dt) override;

		void Create(const std::string& name, int width, int height);
		void BeginFrame();
		void EndFrame();

		void Draw(std::shared_ptr<ag::Texture> texture, const Vector2& position, float angle = 0, const Vector2& scale = Vector2::one);
		void Draw(std::shared_ptr<ag::Texture> texture, const Transform& transform);

		friend class Texture;
	private:
		SDL_Renderer* renderer = nullptr;
		SDL_Window* window = nullptr;
	};
}