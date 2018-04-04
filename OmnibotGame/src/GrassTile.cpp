#include "GrassTile.h"
#include "GSettings.h"
#include "glm/gtc/random.hpp"
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
	
	// (!isDirty)return;

	glm::vec3 *flat = (glm::vec3 *)bufferF->mapReplace();

	for (int i = 0; i < grassFlatness.size();i++)
	{
		if (grassFlatness[i].isDirty)
		{
			if (grassFlatness[i].hit)
			{
				grassFlatness[i].angle += 0.1;
				if (grassFlatness[i].angle > 1)grassFlatness[i].angle = 1;
			}
			else
			{
				grassFlatness[i].angle -= grassFlatness[i].friction;
				if (grassFlatness[i].angle < 0){
					grassFlatness[i].angle = 0;
					grassFlatness[i].isDirty = false;
				}
			}

			flatness[i].x = grassFlatness[i].dirX;
			flatness[i].y = grassFlatness[i].dirY;
			flatness[i].z = grassFlatness[i].angle;


		}


		*flat++ = flatness[i];
	}

	bufferF->unmap();

	
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
			if (!grassFlatness[i].hit){

				glm::vec2 hitDir = vec2(posIn.x, posIn.y) - pos;
				grassFlatness[i].hit = true;
				grassFlatness[i].isDirty = true;
				grassFlatness[i].dirX = hitDir.y;
				grassFlatness[i].dirY = -hitDir.x;
				
			}

		}
		else
		{
			grassFlatness[i].hit = false;
		}
		

		
	}
	


}
void GrassTile::addGrass(Grass pos)
{
	pos.position.x -= xRWorld;
	pos.position.y -= yRWorld;
	positions.push_back(pos);
	GrassFlatness f;
	f.friction = glm::linearRand(0.004, 0.01);
	grassFlatness.push_back(f);
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
