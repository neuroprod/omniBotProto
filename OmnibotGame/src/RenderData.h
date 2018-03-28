#pragma once

#include "cinder\app\App.h"
#include "ProjectionCamera.hpp"
#include "Shadow.h"
#include "cinder/gl/gl.h"
class RenderData;
typedef std::shared_ptr<RenderData> RenderDataRef;

class RenderData
{

public: 
	RenderData();
	static RenderDataRef create();
	void setup();
	ci::gl::FboRef getShadowMap();
	
	ProjectionCameraRef camera;
	ShadowRef shadow;

	int playerID;
	ci::gl::TextureCubeMapRef  irradianceCubeMap;
};