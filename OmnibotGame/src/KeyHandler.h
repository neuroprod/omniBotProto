#pragma once
#include "cinder/app/App.h"

class Key
{
public:
	Key(){}

	void setDown()
	{
		if (!isDown)
		{
			isDown = true;
			for (auto f : callbacks){
				f(isDown, code);
			}
		}
	}	
	void setUp()
	{
		if (isDown)
		{
			isDown = false;
			for (auto f : callbacks){
				f(isDown, code);
			}
		}
	}
	void addChangeEvent(std::function<void(bool,int)> fr)
	{
		callbacks.push_back( fr);
	}

	bool isDown = false;
	int code;
	std::vector<std::function<void(bool, int)> >callbacks ;
};

class KeyHandler
{
	static KeyHandler* instance;
	KeyHandler();
public:

	static KeyHandler* getInstance();
	
	void init();
	void update();
	void keyDown(ci::app::KeyEvent event);
	void keyUp(ci::app::KeyEvent event);

	std::vector<Key> keys;

	Key * getKey(int keyCode);
	

};