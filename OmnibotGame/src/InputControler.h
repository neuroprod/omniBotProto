
#pragma once
#include "PlayerInput.h"

class InputControler
{

public:
	InputControler();
	void init();
	void update(double timeEllapsed);
	void draw();
	PlayerInputRef player1Input;
	PlayerInputRef player2Input;
};