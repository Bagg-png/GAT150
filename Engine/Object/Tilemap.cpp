#include "Tilemap.h"
#include "Engine.h"

void ag::Tilemap::Create()
{
	for(int i = 0; i < tiles.size(); i++)
	{
		int index = tiles[i];
		if (index == 0) continue;

		auto actor = ObjectFactory::Instance().Create<Actor>(tileNames[index]);
		if (actor) 
		{
			int x = i % numTilesX;
			int y = i / numTilesX;

			actor->transform.position = start + (ag::Vector2{ x, y } *size);
			scene->AddActor(std::move(actor));
		}
	}
}

bool ag::Tilemap::Write(const rapidjson::Value& value) const
{
	return false;
}

bool ag::Tilemap::Read(const rapidjson::Value& value)
{
	JSON_READ(value, numTilesX);
	JSON_READ(value, numTilesY);
	JSON_READ(value, size);
	JSON_READ(value, start);
	JSON_READ(value, tileNames);
	JSON_READ(value, tiles);

	return true;
}