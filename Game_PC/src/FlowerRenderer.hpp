//
//  FlowerRenderer.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 03/10/2017.
//
//

#ifndef FlowerRenderer_hpp
#define FlowerRenderer_hpp

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"
#include "PlayerLevel.hpp"
class FlowerRenderer
{
    
public:
    FlowerRenderer(){}
    void setup();
    
    ci::gl::GlslProgRef		mGlsl;
    ci::gl::GlslProgRef		mGlslShadow;
    ci::gl::TextureRef texture;
    
    
    void draw(ci::gl::FboRef shadowFBO,ci::mat4 &shadowMatrix,PlayerLevel &playerLevel,double currentTime);
    void drawShadowMap(PlayerLevel &playerLevel,double currentTime);
};


#endif /* FlowerRenderer_hpp */
