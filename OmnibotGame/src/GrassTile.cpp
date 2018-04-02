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
void GrassTile::updateVbo()
{
	
	if (!isDirty)return;

	glm::vec3 *flat = (glm::vec3 *)bufferF->mapReplace();

	for (auto p : flatness)
	{
		*flat++ = p;
	}

	bufferF->unmap();

	isDirty = false;
}
void GrassTile::setPlayerPos(LocalPlayerPos  & playerPos)
{
	
	isDirty = true;
	vec2 pos = playerPos.position;

	float size2 = GSettings::playerRad *  GSettings::playerRad;

	for (int i = 0; i<positions.size();i++)
	{
		vec3 posIn = positions[i].position;

		float dist2 = glm::distance2(pos, vec2(posIn.x, posIn.y));
		if (dist2 < size2)
		{

			
			glm::vec2 hitDir = vec2(posIn.x, posIn.y) - pos;
			

			flatness[i].x = hitDir.y;
			flatness[i].y = hitDir.x;
			flatness[i].z = -3.1315/2;


		}
		
	}
	


}
void GrassTile::addGrass(Grass pos)
{
	pos.position.x -= xRWorld;
	pos.position.y -= yRWorld;
	positions.push_back(pos);
	console() << pos.position<<endl;

	flatness.push_back(vec3(0, 1, 0));
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

	
	




	
	auto layoutF = geom::BufferLayout();
	layoutF.append(geom::Attrib::TANGENT, 3, sizeof(vec3), 0);
	bufferF = gl::Vbo::create(GL_ARRAY_BUFFER, flatness, GL_DYNAMIC_DRAW);
	

	// construct a VAO that describes the data in the buffer according to your layout.
	mVboMesh = gl::VboMesh::create(positions.size(), GL_POINTS, { { layoutP, bufferP }, { layoutF, bufferF } }); //, { layoutC, bufferC }, { layoutU, bufferU }, { layoutF, bufferF }});
}
