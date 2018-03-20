#include "PlayerInput.h"

#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;


PlayerInput::PlayerInput()
{

}
PlayerInputRef PlayerInput::create()
{
	return std::make_shared<PlayerInput>();
}
void PlayerInput::init(int tl, int t, int tr, int r, int br, int b, int bl, int l)
{
	KeyHandler * keyHandler = KeyHandler::getInstance();
	keys.push_back(keyHandler->getKey(tl));
	keys.push_back(keyHandler->getKey(t));
	keys.push_back(keyHandler->getKey(tr));
	keys.push_back(keyHandler->getKey(r));
	keys.push_back(keyHandler->getKey(br));
	keys.push_back(keyHandler->getKey(b));
	keys.push_back(keyHandler->getKey(bl));
	keys.push_back(keyHandler->getKey(l));

	for (int i = 0; i < keys.size();i++)
	{
		keys[i]->addChangeEvent([&, i](bool isDown, int code){if (isDown){ setDown(i); } else{ setUp(i); }});
	}

	float cos45 = cos(3.1415 / 4);

	dirs.push_back(vec3(-cos45,0, -cos45));
	dirs.push_back(vec3(0,0, -1));
	dirs.push_back(vec3(cos45,0, -cos45));
	dirs.push_back(vec3(1,0, 0));
	dirs.push_back(vec3(cos45,0, cos45));
	dirs.push_back(vec3(0,0, 1));
	dirs.push_back(vec3(-cos45,0, cos45));
	dirs.push_back(vec3(-1,0, 0));

}
void PlayerInput::setUp(int index)
{
	if (index == currentIndex)
	{
		currentIndex = -1;
		targetSpeed.x = 0;
		targetSpeed.y = 0;
		targetSpeed.x = 0;
		targetSpeed.z = 0;
	}
}

void PlayerInput::setDown(int index)
{
	currentIndex = index;
	targetSpeed = dirs[currentIndex];
	targetSpeed *= 10;
} 
void PlayerInput::update(double delta)
{
	float maxAccel =12222222.f*delta;


	float errorX = currentSpeed.x - targetSpeed.x;
	if (abs(errorX) < maxAccel)
	{
		currentSpeed.x = targetSpeed.x;
	}
	else
	{
		if (errorX > 0)
		{
			currentSpeed.x -= maxAccel;
		}
		else
		{
			currentSpeed.x += maxAccel;
		}
	}


	float errorZ = currentSpeed.z - targetSpeed.z;
	if (abs(errorZ) < maxAccel)
	{
		currentSpeed.z = targetSpeed.z;
	}
	else
	{
		if (errorZ > 0)
		{
			currentSpeed.z -= maxAccel;
		}
		else
		{
			currentSpeed.z += maxAccel;
		}
	}
	currentSpeedDelta = currentSpeed*(float)delta;
	
}
void PlayerInput::draw()
{
	ci::gl::drawStringCentered(name,vec2());
	ci::gl::drawLine(vec2(0, 0), vec2(currentSpeed.x*10.f, currentSpeed.z*10.f));
}