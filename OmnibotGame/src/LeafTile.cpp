
#include "LeafTile.h"
#include "GSettings.h"
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
	console() << leafs.size() << endl;
	for (LeafRef l: leafs)
	{
		float dist2 = glm::distance2(pos, vec2(l->position.x, l->position.y));
		if (dist2 < size2)
		{
			l->rotation.x += 0.1;
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
