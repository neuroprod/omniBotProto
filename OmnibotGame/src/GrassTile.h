#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Grass.h"
#include "GrassFlatness.h"
#include "LocalPlayerPos.h"
class GrassTile;
typedef std::shared_ptr<GrassTile> GrassTileRef;


class GrassTile
{

public:
	GrassTile();
	static GrassTileRef create();
	void addGrass(Grass grass);
	void setup(int posX, int posY);

	void setPlayerPos(LocalPlayerPos  & playerPos);
	void make();

	void draw();

	ci::gl::VboMeshRef	mVboMesh;
	std::vector<Grass> positions;
	std::vector<ci::vec3> flatness;
	std::vector<GrassFlatness> grassFlatness;
	ci::gl::VboRef bufferF;


	void updateVbo();
	bool isDirty;

	float xR;
	float yR;

	float size;

	float xRWorld;
	float yRWorld;

};