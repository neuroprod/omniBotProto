#include "KeyHandler.h"



using namespace ci;
using namespace ci::app;
using namespace std;



KeyHandler::KeyHandler()
{

}


KeyHandler* KeyHandler::instance = 0;
KeyHandler* KeyHandler::getInstance()
{
	if (instance == 0)
	{
		instance = new KeyHandler();
		instance->init();
	}
	return instance;
	
}

void KeyHandler::update()
{
	
}
void KeyHandler::init()
{
	keys.resize(310);
	for (int i = 0; i < keys.size(); i++)
	{
		keys[i].code = i;

	}

	getWindow()->getSignalKeyDown().connect((std::bind(&KeyHandler::keyDown, this, std::placeholders::_1)));
	getWindow()->getSignalKeyUp().connect((std::bind(&KeyHandler::keyUp, this, std::placeholders::_1)));
}

void KeyHandler::keyDown(ci::app::KeyEvent event)
{
	keys[event.getCode()].setDown();
}

void KeyHandler::keyUp(ci::app::KeyEvent event)
{
	keys[event.getCode()].setUp();
}

Key * KeyHandler::getKey(int keyCode)
{
	return   &keys[keyCode];
}