#pragma once
#include "cinder/app/App.h"

class NormalGenerator
{


public:
	NormalGenerator(){}

	std::vector<ci::vec3> getNormals(std::vector<ci::vec3>positions, std::vector<unsigned short>indices);



};