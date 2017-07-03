//
//  Renderer.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 14/06/2017.
//
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "ProjectionCamera.hpp"

const int	FBO_WIDTH = 2048;
const int	FBO_HEIGHT = 2048;


class GameRenderer
{

public:
    GameRenderer(){};
    void setup();
    
    
    void startShadowDraw(int id);
    void stopShadowDraw(int id);
    
    void startMainDraw();
    void stopMainDraw();
    
    void setupShadow();
    void updateCameraPosition();
    
    ci::CameraPersp camera;
    ProjectionCamera cameraProj;
    
    
    ci::gl::FboRef			mFbo1;
    ci::gl::FboRef			mFbo2;
    ci::CameraPersp			mLightCam;
    ci::vec3				mLightPos;
    ci::gl::Texture2dRef	mShadowMapTex;
    ci::gl::Texture2dRef	mShadowMapTex2;
    
    float screenHeight= 720;
    float screenWidth =1280 ;
    
    float off=0;
    float offyCam = 1800;
    float offzCam = -2000;
    
    glm::vec3 cameraPosition;
    
    
    ci::gl::BatchRef		mFloorShadowedBatch;
    ci::gl::GlslProgRef		mGlslFloor ;


    glm::vec3 mvLightPos	;
    glm::mat4 shadowMatrix;
};
#endif /* Renderer_hpp */
