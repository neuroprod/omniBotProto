#include "Player.h"
#include "cinder/gl/gl.h"


using namespace std;
using namespace ci::app;
using namespace ci;


Player::Player()
{

}

PlayerRef Player::create()
{
	return  make_shared<Player>();
}
void Player::update()
{
	if (glm::length2(controlesInput->currentSpeed) == 0)
	{
		isMoving = false;
	}
	else
	{
		isMoving = true;
	}

	vec4 currentSpeedRot = rotMatrix *vec4(controlesInput->currentSpeed.x, 0, controlesInput->currentSpeed.z, 1.f);

	levelPositionVirtual += vec3(currentSpeedRot.x, 0, currentSpeedRot.z);
	if (levelPositionVirtual.x < 0)levelPositionVirtual.x += worldSize;
	if (levelPositionVirtual.x > worldSize)levelPositionVirtual.x -= worldSize;
	if (levelPositionVirtual.z < 0)levelPositionVirtual.z += worldSize;
	if (levelPositionVirtual.z > worldSize)levelPositionVirtual.z -= worldSize;
	

	float centerDistanceNow = glm::length(screenPositionVirtual);
	float centerDistanceNew = glm::length(screenPositionVirtual + controlesInput->currentSpeed);

	float moveFactor = 1;
	if (centerDistanceNew > centerDistanceNow){
		if (centerDistanceNow > 200)
		{
			moveFactor = max(0.f, 1.f - ((centerDistanceNow - 200.f) / 50.f));
		}
	}
	screenPositionVirtual += controlesInput->currentSpeed*moveFactor;




}
void Player::setScreenPosition(vec3 pos)
{
	screenPositionVirtual = pos;
	screenPosition = pos;
	screenPosition.x += 0;
	screenPosition.y +=0;
}
void Player::setLevelPosition(vec3 pos)
{
	levelPositionVirtual = pos;
	levelPosition = pos;

}

void Player::resolveScreenMatrix(PlayerRef other)
{
	//if (!isMoving)return;

	posOther = getClosestLevelPosition(levelPositionVirtual, other->levelPositionVirtual);
	float screenDistance = glm::distance(screenPositionVirtual, other->screenPositionVirtual);
	if (currentDistance <= screenDistance)
	{
	
		float angleLevel = atan2f(levelPositionVirtual.z - posOther.z, levelPositionVirtual.x - posOther.x);
		float angleScreen = atan2f(screenPositionVirtual.z - other->screenPositionVirtual.z, screenPositionVirtual.x - other->screenPositionVirtual.x);
		angle = angleLevel - angleScreen;
		//if (!other->isMoving)angle = angleLevel;

		
		//angle = 0;
		//console() << "FOUND HIM" << endl;
		
	
	
	} else 
	{
	
		angle = 0;
	
	}
	glm::mat4 centerMatrix = glm::mat4();
	centerMatrix = translate(centerMatrix, vec3(1280 / 2, 720 / 2, 0));


	screenMatrix = glm::mat4();
	//
	screenMatrix = translate(screenMatrix, vec3(+screenPositionVirtual.x, +screenPositionVirtual.z, 0));
	screenMatrix = glm::rotate(screenMatrix, -angle, vec3(0, 0, 1));
	screenMatrix = translate(screenMatrix, vec3(-screenPositionVirtual.x, -screenPositionVirtual.z, 0));
	screenMatrix = translate(screenMatrix, vec3(-levelPositionVirtual.x + screenPositionVirtual.x, -levelPositionVirtual.z + screenPositionVirtual.z, 0));
	screenMatrixInv = glm::inverse(screenMatrix);


	screenMatrix = centerMatrix*screenMatrix;

	rotMatrix = glm::mat4();
	


}
ci::vec3  Player::getClosestLevelPosition(ci::vec3 posMe, ci::vec3 posOther)
{

	vec3 posNear = posOther;
	float distMin = 10000000;
	for (int x = -1; x <= 1; x++)
	{
		for (int z = -1; z <= 1; z++)
		{
			vec3 testPos = posOther;
			testPos.x += x*worldSize;
			testPos.z += z*worldSize;

			float dist  =glm::distance2(posMe, testPos);
			if (dist < distMin)
			{
				distMin = dist;
				posNear = testPos;
			}
		}
	}
	currentDistance =sqrt(distMin);
	return posNear;
}
void Player::drawVirtual()
{

	gl::drawLine(vec2(levelPositionVirtual.x, levelPositionVirtual.z), vec2(posOther.x, posOther.z));
	gl::drawStrokedCircle(vec2(levelPositionVirtual.x, levelPositionVirtual.z), size/2);

}
void Player::draw()
{

	
	gl::drawStrokedCircle(vec2(levelPosition.x, levelPosition.z), size);

}

void Player::setTempRealPosition()
{
	//setScreenPosition by camera
	screenPosition += (screenPositionVirtual - screenPosition) / 5.f;

	vec4 levelPosition4 = screenMatrixInv*vec4(screenPosition.x, screenPosition.z, 0, 1.0f);
	
	levelPosition.x = levelPosition4.x ;
	
	levelPosition.z = levelPosition4.y ;

	

}