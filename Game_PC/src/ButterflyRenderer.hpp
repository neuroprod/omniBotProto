//
//  ButterflyRenderer.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 03/10/2017.
//
//

#ifndef ButterflyRenderer_hpp
#define ButterflyRenderer_hpp
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Butterfly.hpp"
#include "PlayerLevel.hpp"
class ButterflyRenderer
{

public:
    ButterflyRenderer(){};
    void setup(int tileSize,int numTiles, std::vector<LevelTileRef> tiles);
    void draw(PlayerLevel &playerLevel,double currentTime,ci::vec3 lightPos);
    void update();
    std::vector<ButterflyRef> butterflys;
    ci::Surface dataSurface;
    ci::vec2 move;
    float fullSize;
     std::vector<LevelTileRef> tiles;
    int numTiles;
    int tileSize;
    
    
    ci::gl::GlslProgRef		mGlsl;
  
    
    
   /* void draw(ci::gl::FboRef shadowFBO,ci::mat4 &shadowMatrix,PlayerLevel &playerLevel,double currentTime);
    void drawShadowMap(PlayerLevel &playerLevel,double currentTime);
*/
  
};

#endif /* ButterflyRenderer_hpp */
