#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/gl/Texture.h"
#include "LeafTile.h"
#include "Leaf.h"
#include "RenderData.h"
#include "LocalPlayerPos.h"
struct VertexData {

	ci::vec3 position;
	ci::vec3 normal;
};


class LeafHandler
{

public:
	LeafHandler();
	void setup();
	void updateGL();
	void resolvePlayer(std::vector<LocalPlayerPos> &playerPositions);
	void draw(std::vector<int>&indices, std::vector<ci::vec2> &positions, RenderDataRef renderdata);
	void drawDepth(std::vector<int>&indices, std::vector<ci::vec2> &positions, RenderDataRef renderdata);
	void buildLeafs();
	void setupRendering();

	ci::gl::GlslProgRef		mGlslDepth;
	ci::gl::GlslProgRef		mGlsl;
	ci::gl::TextureRef		gradientMap;
	
	void buildTiles();
	std::vector<LeafTileRef> tiles;



	
	ci::gl::VboMeshRef buildVBOMesh();
	
};