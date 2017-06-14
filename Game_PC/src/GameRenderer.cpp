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



void GameRenderer::startShadowDraw()
{
    gl::enableDepthRead();
    gl::enableDepthWrite();
    gl::enable( GL_POLYGON_OFFSET_FILL );
    glPolygonOffset( 2.0f, 2.0f );
    
 
    mFbo->bindFramebuffer();
    gl::viewport(vec2( 0.0f ), mFbo->getSize());
    gl::color( Color::white() );
     gl::clear( Color::white() );
    gl::pushMatrices();
    gl::setMatrices( mLightCam );

}
void GameRenderer::stopShadowDraw()
{

    gl::popMatrices();
    mFbo->unbindFramebuffer();
    gl::disable( GL_POLYGON_OFFSET_FILL );
    gl::disableDepthRead();
    gl::disableDepthWrite();
    gl::viewport(vec2( 0.0f ),vec2( screenWidth ,screenHeight ));

}
void GameRenderer::startMainDraw()
{

    gl::enableDepthRead();
    gl::enableDepthWrite();
     gl::pushMatrices();
    gl::setMatrices( cameraProj);
    
    mShadowMapTex->bind ((uint8_t) 0 );
    
    mvLightPos	= vec3( gl::getModelView() * vec4( mLightPos, 1.0f ) ) ;
    shadowMatrix = mLightCam.getProjectionMatrix() * mLightCam.getViewMatrix();
    
    mGlslFloor ->uniform( "uShadowMap", 0 );
    mGlslFloor ->uniform( "uLightPos", mvLightPos );
    mGlslFloor ->uniform( "uShadowMatrix", shadowMatrix );
    
    
    
    gl::pushMatrices();
    gl::translate(screenWidth/2, screenHeight/2, +10);
    gl::rotate(3.1415/2,vec3(1.f,0.f,0.f) );
    mFloorShadowedBatch->draw();
    gl::popMatrices();
    
    
    

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
    mLightPos.x=screenWidth / 2+500;
    mLightPos.y=screenHeight / 2+500;
    mLightPos.z=-1000;
    
    gl::Texture2d::Format depthFormat;
    
    
    depthFormat.setInternalFormat( GL_DEPTH_COMPONENT32F );
    depthFormat.setCompareMode( GL_COMPARE_REF_TO_TEXTURE );
    depthFormat.setMagFilter( GL_LINEAR );
    depthFormat.setMinFilter( GL_LINEAR );
    depthFormat.setWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
    
    depthFormat.setCompareFunc( GL_LEQUAL );
    
    mShadowMapTex = gl::Texture2d::create( FBO_WIDTH, FBO_HEIGHT, depthFormat );
    
    
    gl::Fbo::Format fboFormat;
    fboFormat.attachment( GL_DEPTH_ATTACHMENT, mShadowMapTex );
    mFbo = gl::Fbo::create( FBO_WIDTH, FBO_HEIGHT, fboFormat );
    
    // Set up camera from the light's viewpoint
    mLightCam.setPerspective( 100.0f, mFbo->getAspectRatio(), 300.f,2000.0f );
    mLightCam.lookAt( mLightPos, vec3( screenWidth / 2,screenHeight / 2,0) );
    
    mGlslFloor = gl::GlslProg::create( loadAsset( "floor_shader.vert" ), loadAsset( "floor_shader.frag" ) );
    auto floor				= geom::Cube().size( 1000.0f, 0.5f, 1000.0f );
    
    mFloorShadowedBatch		= gl::Batch::create( floor, mGlslFloor );
    
}
