#include "GrassTile.h"
#include "GSettings.h"
using namespace std;
using namespace ci;
using namespace ci::app;



GrassTile::GrassTile()
{

}
GrassTileRef GrassTile::create()
{
	return make_shared<GrassTile>();
}

void GrassTile::addGrass(Grass pos)
{
	pos.position.x -= xRWorld;
	pos.position.y -= yRWorld;
	positions.push_back(pos);
	
}

void  GrassTile::setup(int x, int y)
{
	xR = x;
	yR = y;
	float size = GSettings::tileSize;
	xRWorld = xR*size;
	yRWorld = yR*size;

}
void GrassTile::draw()
{

	gl::draw(mVboMesh);
}

void  GrassTile::make(){


	auto layoutP = geom::BufferLayout();

	layoutP.append(geom::Attrib::POSITION, 3, sizeof(Grass), offsetof(Grass, position));
	layoutP.append(geom::Attrib::NORMAL, 3, sizeof(Grass), offsetof(Grass, normal));
	auto bufferP = gl::Vbo::create(GL_ARRAY_BUFFER, positions, GL_STATIC_DRAW);

	
	




	/*
	auto layoutF = geom::BufferLayout();
	layoutF.append(geom::Attrib::TEX_COORD_1, 1, sizeof(float), 0);
	auto bufferF = gl::Vbo::create(GL_ARRAY_BUFFER, flat, GL_DYNAMIC_DRAW);
	*/

	// construct a VAO that describes the data in the buffer according to your layout.
	mVboMesh = gl::VboMesh::create(positions.size(), GL_POINTS, { { layoutP, bufferP } }); //, { layoutC, bufferC }, { layoutU, bufferU }, { layoutF, bufferF }});
}