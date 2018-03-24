#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/VboMesh.h"
#include "Leaf.h"
#include "cinder/gl/gl.h"
class LeafTile;
typedef std::shared_ptr<LeafTile> LeafTileRef;


class LeafTile
{

public:
	LeafTile();
	static LeafTileRef create();

	void setup(int posX, int posY);

	void addLeaf(LeafRef l);

	void makeBatch(ci::gl::VboMeshRef meshRef, ci::gl::GlslProgRef		mGlsl);

	ci::gl::VboRef			mInstanceDataVbo;
	ci::gl::VboRef			mInstanceDataVboColor;
	ci::gl::BatchRef		mBatch;

	std::vector<LeafRef> leafs;
	void draw();


	float xR;
	float yR;

	float size;

	float xRWorld;
	float yRWorld;
};