#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "InputControler.h"
#include "Player.h"
#include "World.h"
using namespace ci;
using namespace ci::app;
using namespace std;

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
};

void OmnibotGameApp::setup()
{
	setWindowSize(1280, 720);
	setFrameRate(120);
	
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



}
void OmnibotGameApp::updateVirtualPlayers()
{

	player1->update();
	player2->update();

	vec2 player1Screen = vec2(player1->screenPositionVirtual.x, player1->screenPositionVirtual.z);
	vec2 player2Screen = vec2(player2->screenPositionVirtual.x, player2->screenPositionVirtual.z);

	vec2 distVec = player1Screen - player2Screen;
	float distance = glm::length(distVec);

	vec2 perpVec = vec2(-distVec.y, distVec.x);

	vec2 centerPos = (player1Screen + player2Screen) / 2.f;
	vec2 perpVecNorm = glm::normalize(perpVec);

	pointCenter1 = centerPos - perpVecNorm*720.f;
	pointCenter2 = centerPos + perpVecNorm*720.f;

	vec2 distVecNorm = glm::normalize(distVec);

	pointCenter1off = pointCenter1 + distVecNorm*720.f;
	pointCenter2off = pointCenter2 + distVecNorm*720.f;


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
	gl::clear( Color( 0, 0, 0 ) ); 



	gl::pushMatrices();
	gl::translate(getWindowCenter());
	gl::drawStrokedCircle(vec2(0, 0), getWindowHeight() / 2);
	gl::drawLine(pointCenter1, pointCenter2);
	gl::popMatrices();
	
	drawVirtualPosition();
	

	gl::pushMatrices();
	gl::setModelMatrix(player1->screenMatrix);
	player1->draw();
	gl::popMatrices();

	gl::pushMatrices();
	gl::setModelMatrix(player2->screenMatrix);
	player2->draw();
	gl::popMatrices();

	//draw debug
	inputControler.draw();
	gl::drawString(toString(getAverageFps()), vec2(0, 0));
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

CINDER_APP( OmnibotGameApp, RendererGl )
