#pragma once
#include "cinder/app/App.h"
#include "PlayerInput.h"
class Player;
typedef std::shared_ptr<Player> PlayerRef;


class Player
{

public:
	Player();
	static PlayerRef create();


	void update();
	void drawVirtual();
	void draw();


	void setScreenPosition(ci::vec3 pos);
	void setLevelPosition(ci::vec3 pos);

	ci::vec3  getClosestLevelPosition(ci::vec3 posMe, ci::vec3 posOther);

	int id;

	float worldSize;
	void resolveScreenMatrix(PlayerRef other);

	float size = 50;

	PlayerInputRef controlesInput;
	bool isMoving = false;
	ci::vec3 levelPositionVirtual;
	ci::vec3 screenPositionVirtual;

	ci::mat4 screenMatrix;//virtual screen matrix
	ci::mat4 screenMatrixInv;

	ci::mat4 rotMatrix;
	float angle = 0;

	ci::vec3 posOther;
	float currentDistance;


	ci::vec3 levelPosition;
	ci::vec3 screenPosition;


	void setTempRealPosition();

};