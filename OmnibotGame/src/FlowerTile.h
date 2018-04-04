#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Flower.h"
#include "GrassFlatness.h"
#include "LocalPlayerPos.h"
class FlowerTile;
typedef std::shared_ptr<FlowerTile> FlowerTileRef;


class FlowerTile
{

public:
	FlowerTile();
	static FlowerTileRef create();
	void addFlower(Flower Flower);
	void setup(int posX, int posY);
	void make();

	void draw();

	ci::gl::VboMeshRef	mVboMesh;
	std::vector<Flower> positions;

	std::vector<ci::vec3> flatness;
	std::vector<GrassFlatness> grassFlatness;
	ci::gl::VboRef bufferF;

	void setPlayerPos(LocalPlayerPos  & playerPos);
	void updateVbo();

	float xR;
	float yR;

	float size;

	float xRWorld;
	float yRWorld;
	bool isDirty;
};