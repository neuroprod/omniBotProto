#pragma once
#include "cinder/app/App.h"
#include "KeyHandler.h"

class PlayerInput;
typedef std::shared_ptr<PlayerInput> PlayerInputRef;

class PlayerInput
{
public:
	static PlayerInputRef create();

	PlayerInput();
	void init(int tl, int t, int tr, int r, int br, int b, int bl,int l);
	void update(double delta);
	void draw();

	void setUp(int index);
	void setDown(int index);
	

	std::string name;

	int currentIndex = -1;
	ci::vec3 currentSpeed=ci::vec3(0,0,0);
	ci::vec3 targetSpeed=ci::vec3(0,0,0);
	ci::vec3 currentSpeedDelta = ci::vec3(0, 0, 0);

	std::vector<Key *> keys;
	std::vector<ci::vec3> dirs;
};