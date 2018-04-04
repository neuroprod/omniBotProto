#pragma once
struct GrassFlatness {

	bool hit = false;
	bool isDirty = false;
	float dirX;
	float dirY;
	float angle = 0;
	float friction = 0.1;
};