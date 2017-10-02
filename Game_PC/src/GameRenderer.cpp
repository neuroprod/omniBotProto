//
//  GameRenderer.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 14/06/2017.
//
//

#include "GameRenderer.hpp"


using namespace std;
using namespace ci;
using namespace ci::app;


void GameRenderer::setup()
{


     
    setupShadow();
    
    
    updateCameraPosition();






}



void GameRenderer::startShadowDraw(int id)
{
    gl::enableDepthRead();
    gl::enableDepthWrite();
    gl::enable( GL_POLYGON_OFFSET_FILL );
    glPolygonOffset( 2.0f, 2.0f );
    
    if(id==0){
        mFbo1->bindFramebuffer();
        gl::viewport(vec2( 0.0f ), mFbo1->getSize());
    }
    else
    {
        mFbo2->bindFramebuffer();
        gl::viewport(vec2( 0.0f ), mFbo2->getSize());
    }
    gl::color( Color::white() );
    gl::clear( Color::white() );
    gl::pushMatrices();
    
       // Set up camera from the light's viewpoint
    mLightCam.setPerspective( 20.0f, mFbo1->getAspectRatio(), 1.f,5000.0f );
    mLightCam.setWorldUp(vec3(0,0,1));
    mLightCam.lookAt( mLightPos, vec3( screenWidth / 2,screenHeight / 2,0),vec3(0,0,1) );

    
    gl::setMatrices( mLightCam);

}
void GameRenderer::stopShadowDraw(int id)
{

    gl::popMatrices();
    if(id==0){
         mFbo1->unbindFramebuffer();
    }
    else
    {
        mFbo2->unbindFramebuffer();
    }
    gl::disable( GL_POLYGON_OFFSET_FILL );
    gl::disableDepthRead();
    gl::disableDepthWrite();
    gl::viewport(vec2( 0.0f ),vec2( screenWidth ,screenHeight ));

}
void GameRenderer::startMainDraw()
{
    shadowMatrix = mLightCam.getProjectionMatrix() * mLightCam.getViewMatrix();
    
    gl::enableDepthRead();
    gl::enableDepthWrite();
     gl::pushMatrices();
    gl::setMatrices( cameraProj);
    
     
    

}
void GameRenderer::stopMainDraw()
{
    gl::popMatrices();

    gl::disableDepthRead();
    gl::disableDepthWrite();
}




void GameRenderer::updateCameraPosition()
{
    
    cameraPosition.x=0;
    cameraPosition.y=-screenHeight-offyCam ;
    cameraPosition.z=offzCam;
    
    cameraProj.setEyePoint(vec3(screenWidth / 2, screenHeight+offyCam /2,offzCam/2));
    
    cameraProj.lookAt(vec3(screenWidth / 2,screenHeight+offyCam /2, 0));
    cameraProj.setCameraPos(cameraPosition);
    
    
    
}
void GameRenderer::setupShadow()
{
   
    
    gl::Texture2d::Format depthFormat;
    
    
    depthFormat.setInternalFormat( GL_DEPTH_COMPONENT32F );
    depthFormat.setCompareMode( GL_COMPARE_REF_TO_TEXTURE );
    depthFormat.setMagFilter( GL_LINEAR );
    depthFormat.setMinFilter( GL_LINEAR );
    depthFormat.setWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
    
    depthFormat.setCompareFunc( GL_LEQUAL );
    
    mShadowMapTex = gl::Texture2d::create( FBO_WIDTH, FBO_HEIGHT, depthFormat );
    
    
    gl::Fbo::Format fboFormat;
   // fboFormat.disableColor();
    fboFormat.attachment( GL_DEPTH_ATTACHMENT, mShadowMapTex );
    mFbo1 = gl::Fbo::create( FBO_WIDTH, FBO_HEIGHT, fboFormat );
    
    gl::Texture2d::Format depthFormat2;
    
    
    depthFormat2.setInternalFormat( GL_DEPTH_COMPONENT32F );
    depthFormat2.setCompareMode( GL_COMPARE_REF_TO_TEXTURE );
    depthFormat2.setMagFilter( GL_LINEAR );
    depthFormat2.setMinFilter( GL_LINEAR );
    depthFormat2.setWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
    
    depthFormat2.setCompareFunc( GL_LEQUAL );
    
    mShadowMapTex2 = gl::Texture2d::create( FBO_WIDTH, FBO_HEIGHT, depthFormat2 );
    
    
    gl::Fbo::Format fboFormat2;
    fboFormat2.attachment( GL_DEPTH_ATTACHMENT, mShadowMapTex2 );
    //fboFormat2.disableColor();
    
    mFbo2 = gl::Fbo::create( FBO_WIDTH, FBO_HEIGHT, fboFormat2 );
    
    
    mLightPos.x= screenWidth / 2;
    mLightPos.y=screenHeight / 2+1000;
    mLightPos.z=-2000;
    
    // Set up camera from the light's viewpoint
    mLightCam.setPerspective( 25.0f, mFbo1->getAspectRatio(), 2000.f,5000.0f );
    mLightCam.lookAt( mLightPos, vec3( screenWidth / 2,screenHeight / 2,0));
  
    
}
