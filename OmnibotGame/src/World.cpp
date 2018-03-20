#include "World.h"


World::World()
{

};

void World::setup()
{
	worldSize = tileSize*numTiles;

	for (int y = 0; y<numTiles; y++)
	{
		for (int x = 0; x<numTiles; x++)
		{
			TileRef tile = Tile::create();
			tile->setup(x, y, tileSize);
			
			tiles.push_back(tile);
		}

	}
}
void World::draw()
{
	for (auto t : tiles)
	{
		t->draw();
	}

}