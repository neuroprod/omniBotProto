#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/gl/Texture.h"
#include "LeafTile.h"
#include "Leaf.h"

struct VertexData {

	ci::vec3 position;

};


class LeafHandler
{

public:
	LeafHandler();
	void setup();
	void draw( std::vector<int>&indices, std::vector<ci::vec2> &positions);
	void buildLeafs();
	void setupRendering();

	ci::gl::GlslProgRef		mGlsl;
	ci::gl::TextureRef		gradientMap;
	
	void buildTiles();
	std::vector<LeafTileRef> tiles;



	
	ci::gl::VboMeshRef buildVBOMesh();
	
};