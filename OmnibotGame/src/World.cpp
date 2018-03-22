#include "World.h"
#include "GSettings.h"

World::World()
{

};

void World::setup()
{
	worldSize = GSettings::tileSize*GSettings::numTiles;
	
	for (int y = 0; y<GSettings::numTiles; y++)
	{
		for (int x = 0; x<GSettings::numTiles; x++)
		{
			TileRef tile = Tile::create();
			tile->setup(x, y, GSettings::tileSize);
			
			tiles.push_back(tile);
		}

	}
}
void World::drawPlayerTiles(int index)
{
	
	tiles[index]->draw();

}

void World::draw()
{
	for (auto t : tiles)
	{
		t->drawDebug();
	}

}