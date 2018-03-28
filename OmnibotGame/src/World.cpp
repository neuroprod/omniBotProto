#include "World.h"
#include "GSettings.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace ci::app;
World::World()
{

};

void World::setup()
{

	floorMapping.load();

	worldSize = GSettings::worldSize;
	
	for (int y = 0; y<GSettings::numTiles; y++)
	{
		for (int x = 0; x<GSettings::numTiles; x++)
		{
			TileRef tile = Tile::create();
			tile->setup(x, y, GSettings::tileSize);
			floorMapping.setTileFloorMesh(tile, GSettings::numTiles);
			tiles.push_back(tile);
		}

	}
	leafHandler.setup();
	
}
void World::drawPlayerTiles(int index, RenderDataRef renderdata)
{
	
	vector<int> indices = tiles[index]->friendIndices;
	vector<vec2> positions = tiles[index]->friendPositions;

	floorMapping.draw(tiles, indices, positions,renderdata);
	
	leafHandler.draw(indices, positions,renderdata);

	
	

}
void World::drawPlayerTilesShadow(int index, RenderDataRef renderdata)
{

	vector<int> indices = tiles[index]->friendIndices;
	vector<vec2> positions = tiles[index]->friendPositions;

	
	leafHandler.drawDepth(indices, positions, renderdata);




}
void World::draw()
{
	for (auto t : tiles)
	{
		t->drawDebug();
	}

}