#include "World.h"
#include "GSettings.h"
#include "LocalPlayerPos.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace ci::app;
World::World()
{

};
void World::updateGL()
{
	leafHandler.updateGL();
	grassHandler.updateGL();

}
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
	grassHandler.setup();
	
}

void World::resolvePlayerPos(PlayerRef player)
{
	int index = player->currentTileIndex;
	TileRef playerTile = tiles[index];

	vec2 playerLokalPos;
	playerLokalPos.x = player->levelPosition.x-playerTile->xRWorld;
	playerLokalPos.y = player->levelPosition.y - playerTile->yRWorld;;

	vector<int> indices = tiles[index]->friendIndices;
	vector<vec2> positionAdj = tiles[index]->friendLocalPosAdj;
	float size = GSettings::tileSize;

	vector<LocalPlayerPos> playerPositions;


	for (int i = 0; i < positionAdj.size(); i++)
	{
		TileRef friendTile = tiles[indices[i]];
		vec2 playerTilePos = playerLokalPos + positionAdj[i];
		float dist;
		if (i == 0)
		{
			dist =glm::length(playerLokalPos);
		}
		else if (i == 1)
		{
			dist = playerLokalPos.y;
		}
		else if (i == 2)
		{
			dist = glm::distance(playerLokalPos, vec2(0, size));
		}
		else if (i == 3)
		{
			dist = playerLokalPos.x;
		}
		else if (i == 4)
		{
			dist = 0;
		}
		else if (i == 5)
		{
			dist =size- playerLokalPos.x;
		}
		else if (i == 6)
		{
			dist = glm::distance(playerLokalPos, vec2(size, 0));
		}
		else if (i == 7)
		{
			dist = size - playerLokalPos.y;
		}
		else if (i == 8)
		{
			dist = glm::distance(playerLokalPos, vec2(size, size));
		}

		LocalPlayerPos lpp;
		lpp.dist = dist;
		lpp.position = playerTilePos;
		lpp.index = indices[i];

		playerPositions.push_back(lpp);
	}

	leafHandler.resolvePlayer(playerPositions);
	grassHandler.resolvePlayer(playerPositions);
	

}
void World::drawPlayerTiles(int index, RenderDataRef renderdata)
{
	
	vector<int> indices = tiles[index]->friendIndices;
	vector<vec2> positions = tiles[index]->friendPositions;

	floorMapping.draw(tiles, indices, positions,renderdata);
	
	leafHandler.draw(indices, positions,renderdata);
	grassHandler.draw(indices, positions, renderdata);
	
	

}
void World::drawPlayerTilesShadow(int index, RenderDataRef renderdata)
{

	vector<int> indices = tiles[index]->friendIndices;
	vector<vec2> positions = tiles[index]->friendPositions;

	
	leafHandler.drawDepth(indices, positions, renderdata);
	grassHandler.draw(indices, positions, renderdata);



}
void World::draw()
{
	for (auto t : tiles)
	{
		t->drawDebug();
	}

}