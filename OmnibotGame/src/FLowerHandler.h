#pragma once
#include "FlowerTile.h"
#include "RenderData.h"


class FlowerHandler{

public:
	FlowerHandler();
	void setup();

	void setupRendering();
	void buildTiles();
	void buildFlower();

	std::vector<FlowerTileRef> tiles;

	void draw(std::vector<int>&indices, std::vector<ci::vec2> &positions, RenderDataRef renderdata);
	void drawDepth(std::vector<int>&indices, std::vector<ci::vec2> &positions, RenderDataRef renderdata);

	ci::gl::GlslProgRef		mGlslDepth;
	ci::gl::GlslProgRef		mGlsl;
};