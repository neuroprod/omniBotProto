#pragma once
#include "cinder/app/App.h"
#include "Tile.h"
    
class World
{
public:
	World();

	void setup();
	float tileSize = 256*2;
	int numTiles = 10;
	float worldSize;

	std::vector<TileRef> tiles;
	void draw();
};
