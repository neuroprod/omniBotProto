#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Grass.h"

class GrassTile;
typedef std::shared_ptr<GrassTile> GrassTileRef;


class GrassTile
{

public:
	GrassTile();
	static GrassTileRef create();
	void addGrass(Grass grass);
	void setup(int posX, int posY);
	void make();

	void draw();

	ci::gl::VboMeshRef	mVboMesh;
	std::vector<Grass> positions;





	float xR;
	float yR;

	float size;

	float xRWorld;
	float yRWorld;

};