#pragma once
#include "cinder/app/App.h"
#include "Tile.h"
    
class World
{
public:
	World();

	void setup();
	
	float worldSize;

	std::vector<TileRef> tiles;
	void draw();
	void drawPlayerTiles(int index);
};
