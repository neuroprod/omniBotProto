//
//  GrassRenderer.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 20/06/2017.
//
//

#ifndef GrassRenderer_hpp
#define GrassRenderer_hpp


#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"
#include "PlayerLevel.hpp"
class GrassRenderer
{

public:
    GrassRenderer(){}
    void setup();
 
    ci::gl::GlslProgRef		mGlsl;
    ci::gl::GlslProgRef		mGlslShadow;
     ci::gl::TextureRef texture;
    
    
    void draw(ci::gl::FboRef shadowFBO,ci::mat4 &shadowMatrix,PlayerLevel &playerLevel,double currentTime);
    void drawShadowMap(PlayerLevel &playerLevel,double currentTime);
};

#endif /* GrassRenderer_hpp */
