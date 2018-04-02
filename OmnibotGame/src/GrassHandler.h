#pragma once
#include "GrassTile.h"
#include "RenderData.h"
#include "LocalPlayerPos.h"

class GrassHandler{

public:
	GrassHandler();
	void setup();
	void updateGL();
	void setupRendering();
	void buildTiles();
	void buildGrass();

	std::vector<GrassTileRef> tiles;
	void resolvePlayer(std::vector<LocalPlayerPos> &playerPositions);
	void draw(std::vector<int>&indices, std::vector<ci::vec2> &positions, RenderDataRef renderdata);
	void drawDepth(std::vector<int>&indices, std::vector<ci::vec2> &positions, RenderDataRef renderdata);

	ci::gl::GlslProgRef		mGlslDepth;
	ci::gl::GlslProgRef		mGlsl;
};