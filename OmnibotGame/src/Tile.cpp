#include "Tile.h"
#include "cinder/gl/gl.h"
#include "GSettings.h"

using namespace std;
using namespace ci;
Tile::Tile(){}
TileRef Tile::create()
{
	return make_shared<Tile>();
}

void Tile::setup(int x, int y, float _size)
{
	xR = x;
	zR = y;
	yR = y;
	size = _size;

	xRWorld = xR*size;
	zRWorld = zR*size;
	yRWorld = zR*size;



	for (int yi = -1; yi <= 1; yi++)
	{
	
		for (int xi = -1; xi <= 1; xi++)
		{

			int posX = (x + xi);
			int posY = (y + yi);

			int posXi = posX;
			int posYi = posY;
			if (posXi < 0)posXi += GSettings::numTiles;
			if (posYi < 0)posYi += GSettings::numTiles;

			if (posXi >= GSettings::numTiles)posXi -= GSettings::numTiles;
			if (posYi >= GSettings::numTiles)posYi -= GSettings::numTiles;

			int friendIndex = posXi + posYi*GSettings::numTiles;
			friendIndices.push_back(friendIndex);

			vec2 drawPos;
			drawPos.x = posX*GSettings::tileSize;
			drawPos.y = posY*GSettings::tileSize;
			
			friendPositions.push_back(drawPos);

			vec2 localPosAdj;
			localPosAdj.x = -xi*GSettings::tileSize;
			localPosAdj.y = -yi*GSettings::tileSize;

			friendLocalPosAdj.push_back(localPosAdj);
		}
	
	
	
	
	}


}
void Tile::draw()
{
	gl::drawStrokedRect(Rectf(10, 10,size-20,  size-20));
}

void Tile::drawDebug()
{
	gl::drawStrokedRect(Rectf(xRWorld, zRWorld, xRWorld + size, zRWorld + size));
}