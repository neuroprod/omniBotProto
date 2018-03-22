#pragma once
#include "cinder/app/App.h"

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
	float size;
	float xRWorld;
	float zRWorld;

};