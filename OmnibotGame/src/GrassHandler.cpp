#include "GrassHandler.h"
#include "GSettings.h"
#include "glm/gtc/random.hpp"
#include "cinder/gl/gl.h"
#include "cinder/Perlin.h"
#include "glm/gtc/matrix_transform.hpp"
#include "NormalGenerator.h"
#include "Grass.h"
using namespace std;
using namespace ci;
using namespace ci::app;

GrassHandler::GrassHandler(){}


void GrassHandler::setup()
{
	setupRendering();
	buildTiles();
	buildGrass();
}

void GrassHandler::setupRendering()
{
	mGlsl = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset("grass/grass.vert"))
		.fragment(loadAsset("grass/grass.frag"))
		.geometry(loadAsset("grass/grass.geom")));


	/*mGlsl->uniform("uNoiseMap", 0);
	mGlsl->uniform("uShadowMap", 1);
	texture = gl::Texture::create(loadImage(getAssetPath("noise.jpg")), gl::Texture::Format().wrap(GL_REPEAT));*/

	/*
	mGlslShadow = gl::GlslProg::create(gl::GlslProg::Format().vertex(loadAsset("grass/grassShadow.vert"))
		.fragment(loadAsset("grass/grassShadow.frag"))
		.geometry(loadAsset("grass/grassShadow.geom")));
	mGlslShadow->uniform("uNoiseMap", 0);*/

}
void GrassHandler::buildTiles()
{
	for (int y = 0; y<GSettings::numTiles; y++)
	{
		for (int x = 0; x<GSettings::numTiles; x++)
		{
			GrassTileRef tile = GrassTile::create();
			tile->setup(x, y);

			tiles.push_back(tile);
		}

	}

}
void GrassHandler::buildGrass()
{

	int numGrass = 10000;
	ci::Perlin pnois = Perlin();


	for (int i = 0; i< numGrass; i++)
	{
		

		glm::vec2 randomPos;
		bool found = false;
		while (!found)
		{

			randomPos = glm::vec2(glm::linearRand(0, GSettings::worldSize), glm::linearRand(0, GSettings::worldSize));

			if (pnois.fBm(randomPos.x / 1000, randomPos.y / 1000)<0.0)
			{
				if (pnois.fBm(randomPos.x / 200, randomPos.y / 200) < -0.1)
				{
					found = true;
				}
			}
			

		}

	

		float noise = (pnois.fBm(randomPos.x / 500, randomPos.y / 500) + 0.3f) * 3;


	
		vec2 sRand = glm::diskRand(50.f);
		vec3 position;
		position.x = sRand.x + randomPos.x;
		position.y = sRand.y + randomPos.y;
		if (position.x  < 0)position.x += GSettings::worldSize;
		if (position.y < 0)position.y += GSettings::worldSize;
		if (position.x > GSettings::worldSize)position.x -= GSettings::worldSize;
		if (position.y > GSettings::worldSize)position.y -= GSettings::worldSize;

		int indexX = position.x / GSettings::tileSize;
		int indexY = position.y / GSettings::tileSize;

		indexX %= GSettings::numTiles;
		indexY %= GSettings::numTiles;

		Grass g;
		g.position = position;
		vec2 lookDir = glm::circularRand(1.f);
		g.normal.x = lookDir.x;
		g.normal.y = lookDir.y;
		g.normal.z =- glm::linearRand(50,60);

		int tileIndex = indexX + indexY*GSettings::numTiles;
		tiles[tileIndex]->addGrass(g);
		

	}
	for (auto t : tiles)
	{
		t->make();
	}
}


void GrassHandler::draw(vector<int>&indices, vector<ci::vec2> &positions, RenderDataRef renderdata)
{
	//gl::clear(GL_DEPTH_BUFFER_BIT);
	mGlsl->bind();
	
	mGlsl->uniform("uIrradianceMap", 1);
	renderdata->irradianceCubeMap->bind(1);
	/*
	mGlsl->uniform("uShadowMap", 1);
	renderdata->getShadowMap()->getDepthTexture()->bind(1);

	mGlsl->uniform("uShadowMatrix", renderdata->shadow->shadowMatrix);
	*/
	for (int i = 0; i < indices.size(); i++)
	{
		gl::pushMatrices();
		gl::translate(positions[i]);
		tiles[indices[i]]->draw();
		gl::popMatrices();
	}

	gl::getStockShader(gl::ShaderDef().color())->bind();
}

void GrassHandler::drawDepth(vector<int>&indices, vector<ci::vec2> &positions, RenderDataRef renderdata)
{

	/*mGlslDepth->bind();


	for (int i = 0; i < indices.size(); i++)
	{
		gl::pushMatrices();
		gl::translate(positions[i]);
		tiles[indices[i]]->draw();
		gl::popMatrices();
	}
	gl::getStockShader(gl::ShaderDef().color())->bind();*/
}
