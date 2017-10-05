//
//  FlowerRenderer.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 03/10/2017.
//
//

#include "FlowerRenderer.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;
void FlowerRenderer::setup()
{
    mGlsl = gl::GlslProg::create( gl::GlslProg::Format().vertex( loadAsset( "flower/flower.vert" ) )
                                 .fragment( loadAsset( "flower/flower.frag" ) )
                                 .geometry( loadAsset( "flower/flower.geom" ) ) );
    mGlsl->uniform("uNoiseMap", 0);
    mGlsl->uniform("uShadowMap", 1);
    texture =gl::Texture::create( loadImage(getAssetPath("noise.jpg")),gl::Texture::Format().wrap(GL_REPEAT));
    
    
    mGlslShadow = gl::GlslProg::create( gl::GlslProg::Format().vertex( loadAsset( "flower/flowerShadow.vert" ) )
                                       .fragment( loadAsset( "flower/flowerShadow.frag" ) )
                                       .geometry( loadAsset( "flower/flowerShadow.geom" ) ) );
    mGlslShadow->uniform("uNoiseMap", 0);
    
    
}

void FlowerRenderer::draw(ci::gl::FboRef shadowFBO,ci::mat4 &shadowMatrix,PlayerLevel &playerLevel,double currentTime)
{
    // glDisable(GL_CULL_FACE);
    mGlsl->bind();
    mGlsl->uniform("time", (float)currentTime/20);
    texture->bind(0);
    shadowFBO->getDepthTexture()->bind(1);
    
    mGlsl->uniform( "uShadowMatrix",  shadowMatrix );
    
    gl::pushMatrices();
    gl::translate( playerLevel.player->playerWorldOffset);
    
    for(auto tile : playerLevel.playerTiles)
    {
        tile->drawFlowers();
    }
    
    
    gl::popMatrices();
    
    
    shadowFBO->getDepthTexture()->unbind();
    
    
    
}
void FlowerRenderer::drawShadowMap(PlayerLevel &playerLevel,double currentTime)
{
    mGlslShadow->bind();
    mGlslShadow->uniform("time", (float)currentTime/20);
    texture->bind(0);
    
    gl::pushMatrices();
    gl::translate( playerLevel.player->playerWorldOffset);
    
    for(auto tile : playerLevel.playerTiles)
    {
        tile->drawFlowers();
    }
    
    
    gl::popMatrices();
    
    
    
    texture->unbind();
    
    
}
