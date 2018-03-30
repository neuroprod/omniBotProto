#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/VboMesh.h"
class Tile;
typedef std::shared_ptr<Tile> TileRef;


class Tile
{

public:
	Tile();
	static TileRef create();
	void setup(int x, int z, float size);
	void drawDebug();
	void draw();

	float xR;
	float zR;
	float yR;
	float size;
	float xRWorld;
	float zRWorld;
	float yRWorld;
	std::vector<int> friendIndices;
	std::vector<ci::vec2> friendPositions;
	std::vector<ci::vec2> friendLocalPosAdj;

	ci::gl::VboMeshRef meshFloor;

};