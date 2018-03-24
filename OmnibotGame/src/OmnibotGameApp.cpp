#include "GSettings.h"

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "InputControler.h"
#include "Player.h"
#include "World.h"
#include "ProjectionCamera.hpp"
#include "CinderImGui.h"
using namespace ci;
using namespace ci::app;
using namespace std;

//
// WARNING!!!   Z ==up (don't ask)
//


class OmnibotGameApp : public App {
  public:
	void setup() override;
	void update() override;
	void updateVirtualPlayers() ;
	void drawVirtualPosition() ;
	void draw() override;

	InputControler inputControler;
	double prevTime;

	PlayerRef player1;
	PlayerRef player2;

	World world;

	vec2 pointCenter1;
	vec2 pointCenter2;

	vec2 pointCenter1off;
	vec2 pointCenter2off;
	ProjectionCamera camera;
};

void OmnibotGameApp::setup()
{
	setWindowSize(GSettings::windowWidth, GSettings::windowHeight);
	setFrameRate(120);
	ImGui::initialize();

	inputControler.init();
	world.setup();


	player1 = Player::create();
	player1->id = 1;
	player1->worldSize = world.worldSize;
	player1->setScreenPosition(vec3(-200, 0, 0));
	player1->setLevelPosition(vec3(0, 0, 0));
	player1->controlesInput = inputControler.player1Input;


	player2 = Player::create();
	player2->id = 2;
	player2->worldSize = world.worldSize;
	player2->setScreenPosition(vec3(200, 0, 0));
	player2->setLevelPosition(vec3(200, 0, 500));
	player2->controlesInput = inputControler.player2Input;

	
	camera.updateSetting();



	


}





void OmnibotGameApp::update()
{
	
	double currentTime = getElapsedSeconds();
	double delta = currentTime - prevTime;
	prevTime = currentTime;
	//slot1:

	//1:start get real position
	//do calibration to game world

	//2:update player
	inputControler.update(delta);
	updateVirtualPlayers();


	//task2
	//send Robot targetPosition(player screenpostion) 

	//update real game
	//should come from camera
	player1->setTempRealPosition();
	player2->setTempRealPosition();


	vec2 player1Screen = vec2(player1->screenPosition.x, player1->screenPosition.z);
	vec2 player2Screen = vec2(player2->screenPosition.x, player2->screenPosition.z);
	vec2 distVec = player1Screen - player2Screen;
	vec2 perpVec = vec2(-distVec.y, distVec.x);
	vec2 centerPos = (player1Screen + player2Screen) / 2.f;
	vec2 perpVecNorm = glm::normalize(perpVec);

	pointCenter1 = centerPos - perpVecNorm*720.f;
	pointCenter2 = centerPos + perpVecNorm*720.f;

	vec2 distVecNorm = glm::normalize(distVec);

	pointCenter1off = pointCenter1 + distVecNorm*720.f;
	pointCenter2off = pointCenter2 + distVecNorm*720.f;

}
void OmnibotGameApp::updateVirtualPlayers()
{

	player1->update();
	player2->update();

	vec2 player1Screen = vec2(player1->screenPositionVirtual.x, player1->screenPositionVirtual.z);
	vec2 player2Screen = vec2(player2->screenPositionVirtual.x, player2->screenPositionVirtual.z);

	vec2 distVec = player1Screen - player2Screen;
	float distance = glm::length(distVec);

	//vec2 perpVec = vec2(-distVec.y, distVec.x);

	


	if (distance<150)
	{

		float offsetDistance = 150 - distance;

		vec2 distVecN = (glm::normalize(distVec)*offsetDistance / 2.f)*1.1f;
		if (player1->isMoving && player2->isMoving){
			player1->screenPositionVirtual.x += distVecN.x;
			player1->screenPositionVirtual.z += distVecN.y;

			player2->screenPositionVirtual.x -= distVecN.x;
			player2->screenPositionVirtual.z -= distVecN.y;
		}
		else if (player2->isMoving)
		{
			player2->screenPositionVirtual.x -= distVecN.x * 2;
			player2->screenPositionVirtual.z -= distVecN.y * 2;
		}
		else if (player1->isMoving)
		{
			player1->screenPositionVirtual.x += distVecN.x * 2;
			player1->screenPositionVirtual.z += distVecN.y * 2;
		}

	}


	player1->resolveScreenMatrix(player2);
	player2->resolveScreenMatrix(player1);
}

void OmnibotGameApp::draw()
{
	gl::color(1, 1, 1);


	gl::clear( Color( 0, 0, 0 ) ); 


	glEnable(GL_STENCIL_TEST);
	gl::clear(Color(0, 0, 0));
	glStencilMask(0xFF);
	gl::clear(GL_STENCIL_BUFFER_BIT);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	glStencilMask(0xFF); // Write to stencil buffer

	
	gl::pushMatrices();
	
	gl::translate(GSettings::windowWidth / 2, GSettings::windowHeight / 2);

	gl::drawSolidTriangle(pointCenter1, pointCenter2, pointCenter1off);
	gl::drawSolidTriangle(pointCenter2, pointCenter2off, pointCenter1off);
	gl::popMatrices();
	glStencilMask(0x00);





	//drawVirtualPosition();
	gl::pushMatrices();

		gl::setMatrices(camera);
		gl::enableDepth(true);

	glStencilFunc(GL_EQUAL, 1, 0xFF);
	gl::clear(Color(0, 0, 0));
	gl::clear(GL_DEPTH_BUFFER_BIT);
		gl::pushMatrices();
			gl::setModelMatrix(player1->screenMatrix);
			player1->draw();
			world.drawPlayerTiles(player1->currentTileIndex);
		gl::popMatrices();

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	gl::clear(GL_DEPTH_BUFFER_BIT);

		gl::pushMatrices();
			gl::setModelMatrix(player2->screenMatrix);
			player2->draw();
			world.drawPlayerTiles(player2->currentTileIndex);
		gl::popMatrices();

	gl::popMatrices();

	glDisable(GL_STENCIL_TEST);
	gl::enableDepth(false);



	gl::pushMatrices();
	gl::translate(getWindowCenter());
	gl::drawStrokedCircle(vec2(0, 0), getWindowHeight() / 2);
	gl::drawLine(pointCenter1, pointCenter2);
	gl::popMatrices();


	//drawVirtualPosition();


	//draw debug
	inputControler.draw();

	//Um::SliderFloat("Min Radius", &minRadius, 1, 499);
	string a = "fps: " + toString(getAverageFps());
	ImGui::Text(a.c_str());
	
}

void OmnibotGameApp::drawVirtualPosition()
{

	

	gl::color(1, 0, 0);
	gl::pushMatrices();

	gl::setModelMatrix(player1->screenMatrix);
	player1->drawVirtual();
	world.draw();

	gl::popMatrices();

	gl::color(0, 1, 0);
	gl::pushMatrices();
	gl::setModelMatrix(player2->screenMatrix);
	player2->drawVirtual();

	world.draw();
	gl::popMatrices();


	gl::color(1, 1, 1);

}

CINDER_APP(OmnibotGameApp, RendererGl(RendererGl::Options().msaa(8).stencil()))
