
#include "LeafTile.h"
#include "GSettings.h"
using namespace std;
using namespace ci;

LeafTile::LeafTile(){}

LeafTileRef  LeafTile::create()
{
	return make_shared<LeafTile>();
}
void LeafTile::addLeaf(LeafRef l)
{
	l->worldOffX = xRWorld;
	l->worldOffY = yRWorld;
	leafs.push_back(l);

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
