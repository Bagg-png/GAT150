#pragma once
#include "Framework/System.h"
#include "AudioChannel.h"
#include <fmod.hpp>
#include <string>
#include <map>

namespace ag
{
	class AudioSystem : public System
	{
	public:
		void StartUp() override;
		void ShutDown() override;

		void Update(float dt);

		void AddAudio(const std::string& name, const std::string& filename);
		AudioChannel PlayAudio(const std::string& name, float volume = 1, float pitch = 1, bool loop = false);

	private:
		FMOD::System* fmodSystem;
		std::map<std::string, FMOD::Sound*> sounds;
	};
}