#pragma once
#include "cinder/app/App.h"
#include "Tile.h"
#include "FloorMapping.hpp"  
#include "LeafHandler.h"
#include "RenderData.h"
class World
{
public:
	World();

	void setup();
	
	float worldSize;

	std::vector<TileRef> tiles;
	void draw();
	void drawPlayerTiles(int index, RenderDataRef renderdata);

	FloorMapping floorMapping;
	LeafHandler leafHandler;
};
