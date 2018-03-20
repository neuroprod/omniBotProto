#include  "InputControler.h"
#include "cinder/gl/gl.h"
using namespace ci;
using namespace ci::app;
using namespace std;

InputControler::InputControler()
{

	
}

void InputControler::init()
{

	//keycodes, (I don't have querty keyboard, so you need to change the codes for your keyboard)
	player1Input = PlayerInput::create();
	player1Input->init(97, 122, 101, 100, 99, 120, 119, 113);
	player1Input->name = "Player 1";

	player2Input = PlayerInput::create();
	player2Input->init(263, 264, 265, 262, 259, 258, 257, 260);
	player2Input->name = "Player 2";
}
void InputControler::update(double delta)
{
	player1Input->update(delta);
	player2Input->update(delta);
	


}
void InputControler::draw()
{
	gl::pushMatrices();

	gl::pushMatrices();
	gl::translate(75, 75);
	player1Input->draw();
	gl::popMatrices();

	gl::pushMatrices();
	gl::translate(getWindowWidth()-75, 75);
	player2Input->draw();
	gl::popMatrices();
	gl::popMatrices();
}