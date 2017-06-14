//
//  ParticleHandler.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 21/05/2017.
//
//

#ifndef ParticleHandler_hpp
#define ParticleHandler_hpp

#include <stdio.h>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/ObjLoader.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"


#include "Particle.hpp"
#include "Player.hpp"
#include "ProjectionCamera.hpp"
#include "GameRenderer.hpp"
struct VertexData {
    ci::vec3 position;
   

    
};



class ParticleHandler
{
    
   
public:
    
    ParticleHandler();
    ~ParticleHandler();
    
    //void updateCameraPosition();
  //  glm::vec3 cameraPosition;
    
    void setup(float radius, glm::vec2 centerPos);
    void update(double elapsed,PlayerRef player1,PlayerRef player2);
    void draw(GameRenderer * renderer);
    void drawShadow(GameRenderer * renderer);
    void reset();
    ci::gl::VboMeshRef buildVBOMesh();
  //  void renderDepthFbo();
   // void setupShadow();
    
    int numParticle =5000;
    std::vector<ParticleRef> particles;
    
    ci::gl::BatchRef		mBatch;
   ci::gl::BatchRef		mBatchPlain;
    
    ci::gl::GlslProgRef		mGlsl;
    ci::gl::GlslProgRef		mGlslPlain;
    ci::gl::VboRef			mInstanceDataVbo;
   ci::gl::VboRef			mInstanceDataVboColor;
    
   /* ci::CameraPersp camera;
    ProjectionCamera cameraProj;
    ci::gl::FboRef			mFbo;
    ci::CameraPersp			mLightCam;
    ci::vec3				mLightPos;
    ci::gl::Texture2dRef	mShadowMapTex;*/
    
    ci::gl::TextureRef		leaveMap;
     ci::gl::TextureRef		gradientMap;
    /*
    float screenHeight= 720;
    float screenWidth =1280 ;
    
    
    float offyCam = 1800;
    float offzCam = -5000;
    
    
    ci::gl::BatchRef		mFloorShadowedBatch;
    ci::gl::GlslProgRef		mGlslFloor ;*/
};

#endif /* ParticleHandler_hpp */
