#include "LeafHandler.h"
#include "GSettings.h"
#include "glm/gtc/random.hpp"
#include "cinder/gl/gl.h"
#include "cinder/Perlin.h"
#include "glm/gtc/matrix_transform.hpp"
#include "NormalGenerator.h"

using namespace std;
using namespace ci;
using namespace ci::app;


LeafHandler::LeafHandler(){}

void LeafHandler::updateGL()
{
	
	for (auto tile : tiles)
	{
		tile->updateVbo();
	}

}
void LeafHandler::resolvePlayer(std::vector<LocalPlayerPos> &playerPositions)
{
	for (LocalPlayerPos  playerPos :  playerPositions)
	{
		if (playerPos.dist < GSettings::playerRad)
		{
			tiles[playerPos.index]->setPlayerPos(playerPos);
		}
	}
}
void LeafHandler::setup()
{
	setupRendering();
	buildTiles();
	buildLeafs();
	
}
void LeafHandler::setupRendering()
{
	gradientMap = gl::Texture::create(loadImage(loadAsset("leafs/gradient.png")));
	try
	{
		mGlsl = gl::GlslProg::create(loadAsset("leafs/leaf_shader.vert"), loadAsset("leafs/leaf_shader.frag"));
	}
	catch (const std::exception& e) {
		console()<<"Shader Failed (leafs)" << e.what() <<endl;
	}
	try
	{
		mGlslDepth = gl::GlslProg::create(loadAsset("leafs/leaf_depth_shader.vert"), loadAsset("leafs/leaf_depth_shader.frag"));
	}
	catch (const std::exception& e) {
		console() << "Shader Failed (leafs depth)" << e.what() << endl;
	}
}
void LeafHandler::buildLeafs()
{
	int numLeafs =GSettings::numLeafs;
	ci::Perlin pnois = Perlin();


	for (int i = 0; i< numLeafs; i++)
	{
		LeafRef p = Leaf::create();

		glm::vec2 randomPos;
		bool found = false;
		while (!found)
		{

			randomPos = glm::vec2(glm::linearRand(0, GSettings::worldSize), glm::linearRand(0, GSettings::worldSize));
			
			if (pnois.fBm(randomPos.x / 1000, randomPos.y / 1000)<0.1)
			{

			}
			else if (pnois.fBm(randomPos.x / 200, randomPos.y / 200) < -0.1)
			{
			
			}
			else
			{
					found = true;
			}
			
		}

		p->friction = glm::linearRand(0.95f, 0.98f);

		float noise = (pnois.fBm(randomPos.x / 500, randomPos.y / 500) + 0.3f) * 3;
		

		p->scale = glm::linearRand(0.5f, 1.5f);

		p->rotation.x = glm::linearRand(0, 7);
		p->rotation.y = glm::linearRand(0, 7);
		p->rotation.z = glm::linearRand(0, 7);
		vec2 sRand = glm::diskRand(150.f);
		p->position.x = randomPos.x + sRand.x;
		p->position.y = randomPos.y + sRand.y;
		p->position.z = glm::linearRand(0.f, -5.f);

		p->positionStart = p->position;




		
		p->color = vec2(noise + glm::linearRand(-0.1f, 0.2f), glm::linearRand(0.3f, 0.8f));
		if (p->position.x < 0)p->position.x += GSettings::worldSize;
		if (p->position.y < 0)p->position.y += GSettings::worldSize;
		if (p->position.x > GSettings::worldSize)p->position.x -= GSettings::worldSize;
		if (p->position.y > GSettings::worldSize)p->position.y -= GSettings::worldSize;

		int indexX = p->position.x / GSettings::tileSize;
		int indexY = p->position.y / GSettings::tileSize;

		indexX %= GSettings::numTiles;
		indexY %= GSettings::numTiles;

		int tileIndex = indexX + indexY*GSettings::numTiles;
		tiles[tileIndex]->addLeaf(p);
		//colors.push_back(vec2(noise + glm::linearRand(-0.1f, 0.2f), glm::linearRand(0.3f, 0.8f)));
		//positions.push_back(p->matrix);

		//particles.push_back(p);

	}
	for (auto t : tiles)
	{
		t->makeBatch(buildVBOMesh(), mGlsl);
	}

}

void LeafHandler::buildTiles()
{
	for (int y = 0; y<GSettings::numTiles; y++)
	{
		for (int x = 0; x<GSettings::numTiles; x++)
		{
			LeafTileRef tile = LeafTile::create();
			tile->setup(x, y);

			tiles.push_back(tile);
		}

	}
}

void LeafHandler::draw(vector<int>&indices, vector<ci::vec2> &positions, RenderDataRef renderdata)
{
	gl::clear(GL_DEPTH_BUFFER_BIT);
	mGlsl->bind();
	mGlsl->uniform("uGradientMap", 0);
	gradientMap->bind(0);
	mGlsl->uniform("uIrradianceMap",2);
	renderdata->irradianceCubeMap->bind(2);

	mGlsl->uniform("uShadowMap", 1);
	renderdata->getShadowMap()->getDepthTexture()->bind(1);

	mGlsl->uniform("uShadowMatrix", renderdata->shadow->shadowMatrix);

	for (int i = 0; i < indices.size(); i++)
	{
		gl::pushMatrices();
		gl::translate(positions[i]);
		tiles[indices[i]]->draw();
		gl::popMatrices();
	}
	
	gl::getStockShader(gl::ShaderDef().color())->bind();
}

void LeafHandler::drawDepth(vector<int>&indices, vector<ci::vec2> &positions, RenderDataRef renderdata)
{

	mGlslDepth->bind();
	

	for (int i = 0; i < indices.size(); i++)
	{
		gl::pushMatrices();
		gl::translate(positions[i]);
		tiles[indices[i]]->draw();
		gl::popMatrices();
	}
	gl::getStockShader(gl::ShaderDef().color())->bind();
}



ci::gl::VboMeshRef  LeafHandler::buildVBOMesh()
{

	vector<vec3> posTemp;

	posTemp.push_back(vec3(0, -7, 3));
	posTemp.push_back(vec3(-3.3, -3.5, 1));
	posTemp.push_back(vec3(3.3, -3.5, 1));
	posTemp.push_back(vec3(-4, 0, 0));
	posTemp.push_back(vec3(4, 0, 0));
	posTemp.push_back(vec3(-3.3, 3.5, 1));
	posTemp.push_back(vec3(3.3, 3.5, 1));
	posTemp.push_back(vec3(0, 7, 3));

	int s = posTemp.size();
	for (int i = 0; i < s; i++)
	{
		posTemp.push_back(posTemp[i]);
	}


	vector<unsigned short> indexTemp;

	indexTemp.push_back(0);
	indexTemp.push_back(2);
	indexTemp.push_back(1);

	indexTemp.push_back(1);
	indexTemp.push_back(2);
	indexTemp.push_back(3);

	indexTemp.push_back(3);
	indexTemp.push_back(2);
	indexTemp.push_back(4);

	indexTemp.push_back(3);
	indexTemp.push_back(4);
	indexTemp.push_back(5);

	indexTemp.push_back(5);
	indexTemp.push_back(4);
	indexTemp.push_back(6);

	indexTemp.push_back(5);
	indexTemp.push_back(6);
	indexTemp.push_back(7);

	int maxIndex = 8;

	s = indexTemp.size();
	for (int i = 0; i < s; i+=3)
	{
		indexTemp.push_back(indexTemp[i] + maxIndex);
		
		indexTemp.push_back(indexTemp[i + 2] + maxIndex);
		indexTemp.push_back(indexTemp[i + 1] + maxIndex);
	}



	NormalGenerator ng;
	vector<vec3> normalsTemp = ng.getNormals(posTemp, indexTemp);



	auto vertices = vector<VertexData>();

	for (int i = 0; i < posTemp.size();i++)
	{
		VertexData right;
		right.position = posTemp[i];
		right.normal = normalsTemp[i];


		vertices.push_back(right);


	}
	auto layout = geom::BufferLayout();

	layout.append(geom::Attrib::POSITION, 3, sizeof(VertexData), offsetof(VertexData, position));
	layout.append(geom::Attrib::NORMAL, 3, sizeof(VertexData), offsetof(VertexData, normal));

	// upload your data to the GPU
	auto buffer = gl::Vbo::create(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);
	ci::gl::VboMeshRef meshRef;
	// construct a VAO that describes the data in the buffer according to your layout.
	meshRef = gl::VboMesh::create(vertices.size(), GL_TRIANGLES, { { layout, buffer } }, indexTemp.size());
	meshRef->bufferIndices(indexTemp.size()*sizeof(unsigned short), indexTemp.data());

	return meshRef;
	 
}
