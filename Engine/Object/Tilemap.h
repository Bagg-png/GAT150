#pragma once
#include "Object/Object.h"
#include "Core/Serializable.h"
#include <vector>

namespace ag 
{
	class Tilemap : public Object, ISerializable {
	public:
		void Create() override;

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		class Scene* scene = nullptr;

		int numTilesX{ 0 };
		int numTilesY{ 0 };
		Vector2 size;
		Vector2 start;
		std::vector<std::string> tileNames;
		//Index for tile array
		std::vector<int> tiles;
	};
}