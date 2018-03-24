#pragma once
#include "cinder/app/App.h"

class Leaf;
typedef std::shared_ptr<Leaf> LeafRef;

class Leaf
{


public:
	Leaf();
	void updateMatrix();
	static LeafRef create();
	glm::vec4 positionStart;
	glm::vec4 position;
	ci::vec3 rotation;
	glm::vec3 speed;
	float scale;
	float friction;
	bool hit;
	glm::mat4 matrix;

	ci::vec2 color;
	float worldOffX;
	float worldOffY;
};