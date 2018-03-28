#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"


const int	FBO_WIDTH = 2048;
const int	FBO_HEIGHT = 2048;


class Shadow;
typedef std::shared_ptr<Shadow> ShadowRef;


class Shadow
{

public:
		Shadow();

		static ShadowRef create();

		void setup();
		void startDraw(int id);
		void stopDraw(int id);

		ci::gl::FboRef			mFbo1;
		ci::gl::FboRef			mFbo2;
		ci::CameraPersp			mLightCam;
		ci::vec3				mLightPos;

		ci::gl::Texture2dRef	mShadowMapTex1;
		ci::gl::Texture2dRef	mShadowMapTex2;

		glm::mat4 shadowMatrix;
};