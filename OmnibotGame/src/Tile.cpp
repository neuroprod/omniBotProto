#include "Tile.h"
#include "cinder/gl/gl.h"
using namespace std;
using namespace ci;
Tile::Tile(){}
TileRef Tile::create()
{
	return make_shared<Tile>();
}

void Tile::setup(int x, int z, float _size)
{
	xR = x;
	zR = z;

	size = _size;

	xRWorld = xR*size;
	zRWorld = zR*size;
}
void Tile::draw()
{
	gl::drawStrokedRect(Rectf(xRWorld, zRWorld, xRWorld + size, zRWorld + size));
}