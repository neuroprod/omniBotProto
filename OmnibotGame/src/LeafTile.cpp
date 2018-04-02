
#include "LeafTile.h"
#include "GSettings.h"
#include "glm/gtc/random.hpp"
using namespace std;
using namespace ci;
using namespace ci::app;

LeafTile::LeafTile(){}

LeafTileRef  LeafTile::create()
{
	return make_shared<LeafTile>();
}
void LeafTile::addLeaf(LeafRef l)
{
	l->position.x -= xRWorld;
	l->position.y -= yRWorld;
	leafs.push_back(l);

}

void LeafTile::setPlayerPos(LocalPlayerPos  & playerPos)
{
	isDirty = true;
	vec2 pos = playerPos.position;
	
	float size2 = GSettings::playerRad *  GSettings::playerRad;
	
	for (LeafRef l: leafs)
	{
		float dist2 = glm::distance2(pos, vec2(l->position.x, l->position.y));
		if (dist2 < size2)
		{

			l->hit = true;
			glm::vec2 hitDir = vec2(l->position.x, l->position.y) - pos;
			float hitSize = GSettings::playerRad- glm::length(hitDir);
			hitDir = glm::normalize(hitDir);
			glm::vec2 moveDir = hitDir*(hitSize );
			l->position.x += moveDir.x;
			l->position.y += moveDir.y;


			
			float rand = glm::linearRand(0.f, 2.f / 30.f);

			l->speed.x += moveDir.x*rand;
			l->speed.y += moveDir.y*rand;
			l->speed.z -= hitSize / 10;
			l->position.z +=l->speed.z;
			
		
			
		}
		if (l->hit)
		{
			l->rotation.x += l->speed.x / 20;
			l->rotation.y +=l->speed.z / 20;

			l->speed *= l->friction;
			l->position.x +=l->speed.x;
			l->position.y += l->speed.y;
			l->position.z += l->speed.z;


		




			if (l->position.z != l->positionStart.z)
			{
				l->speed.z += 0.1;
				if (l->speed.z>2) l->speed.z = 2;
			}

			if (l->position.z> l->positionStart.z)
			{
				l->position.z = l->positionStart.z;
				l->speed.z = 0;
			}
			
			if (length2(l->speed) < 0.1)l->hit = false;

			l->updateMatrix();
		
		
		}
	}



}
void LeafTile::updateVbo()
{
	if (!isDirty)return;

	glm::mat4 *positions = (glm::mat4 *)mInstanceDataVbo->mapReplace();

	for (auto p : leafs)
	{
		*positions++ = p->matrix;
	}

	mInstanceDataVbo->unmap();







	 isDirty = false;
}
void LeafTile::makeBatch(ci::gl::VboMeshRef mesh, ci::gl::GlslProgRef mGlsl)
{

	std::vector<  glm::mat4> positions;
	std::vector<  glm::vec2> colors;

	for (LeafRef l : leafs)
	{
		l->updateMatrix();
		positions.push_back(l->matrix);
		colors.push_back(l->color);
	}





	mInstanceDataVbo = gl::Vbo::create(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::mat4), positions.data(), GL_DYNAMIC_DRAW);
	geom::BufferLayout instanceDataLayout;
	instanceDataLayout.append(geom::Attrib::CUSTOM_0, 16, sizeof(mat4), 0, 1);
	mesh->appendVbo(instanceDataLayout, mInstanceDataVbo);

	mInstanceDataVboColor = gl::Vbo::create(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec2), colors.data(), GL_STATIC_DRAW);
	geom::BufferLayout instanceDataLayoutColor;
	instanceDataLayoutColor.append(geom::Attrib::CUSTOM_1, 2, 0, 0, 1);
	mesh->appendVbo(instanceDataLayoutColor, mInstanceDataVboColor);




	mBatch = gl::Batch::create(mesh, mGlsl, { { geom::Attrib::CUSTOM_0, "vInstancePosition" }, { geom::Attrib::CUSTOM_1, "vInstanceColor" } });



}
void LeafTile::setup(int x,int y)
{
	xR = x;
	yR = y;
	float size =GSettings::tileSize;
	xRWorld = xR*size;
	yRWorld = yR*size;

}
void  LeafTile::draw()
{
	mBatch->drawInstanced(leafs.size());
}
