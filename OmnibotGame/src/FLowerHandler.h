#pragma once
#include "FlowerTile.h"
#include "RenderData.h"
#include "LocalPlayerPos.h"

class FlowerHandler{

public:
	FlowerHandler();
	void setup();

	void setupRendering();
	void buildTiles();
	void buildFlower();
	void updateGL();
	void resolvePlayer(std::vector<LocalPlayerPos> &playerPositions);
	std::vector<FlowerTileRef> tiles;

	void draw(std::vector<int>&indices, std::vector<ci::vec2> &positions, RenderDataRef renderdata);
	void drawDepth(std::vector<int>&indices, std::vector<ci::vec2> &positions, RenderDataRef renderdata);

	ci::gl::GlslProgRef		mGlslDepth;
	ci::gl::GlslProgRef		mGlsl;
	ci::gl::TextureRef		flowerMap;
};