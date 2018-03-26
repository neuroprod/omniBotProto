#pragma once

#include "cinder\app\App.h"
#include "ProjectionCamera.hpp"
#include "cinder/gl/gl.h"
class RenderData;
typedef std::shared_ptr<RenderData> RenderDataRef;

class RenderData
{

public: 
	RenderData();
	static RenderDataRef create();
	void setup();

	ProjectionCameraRef camera;
	ci::gl::TextureCubeMapRef  irradianceCubeMap;
};