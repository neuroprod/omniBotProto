//
//  ButterflyRenderer.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 03/10/2017.
//
//

#include "ButterflyRenderer.hpp"
#include "glm/gtc/random.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;


void ButterflyRenderer::setup(int _tileSize,int _numTiles, std::vector<LevelTileRef> _tiles)
{
    tileSize =_tileSize;
    tiles =_tiles;
    dataSurface =  loadImage(getAssetPath("noise2.png"));
    numTiles =_numTiles;
    fullSize=tileSize*numTiles;
 
    
    
    
    mGlsl = gl::GlslProg::create( gl::GlslProg::Format().vertex( loadAsset( "butterfly/butterfly.vert" ) )
                                 .fragment( loadAsset( "butterfly/butterfly.frag" ) )
                                 .geometry( loadAsset( "butterfly/butterfly.geom" ) ) );

    
    
    
    for(int i=0;i<30;i++)
    {
    
        ButterflyRef b = Butterfly::create();
        b->position= vec3(glm::linearRand(0.f,fullSize),glm::linearRand(0.f,fullSize),-10);
         b->speed= vec3(0,0,0);
        b->noiseOffset= vec2(glm::linearRand(0.f,500.f),glm::linearRand(0.f,500.f));
        b->random=glm::linearRand(0.f,1.0f);
        butterflys.push_back(b);
    
    }


}
void ButterflyRenderer::update()
{
    move.x+=1;
    move.y+=-1;
    
    for(auto tile : tiles)
    {
        tile->butterflys.clear();
    }

    
    
    for(auto b:butterflys)
    {
        
        
        vec2 posNormal =vec2(b->position.x/fullSize,b->position.y/fullSize);
        vec2 pixelPos =mod((posNormal *512.f)+move+ b->noiseOffset,vec2(512.f,512.f));
        
        ColorA color1= dataSurface.getPixel(pixelPos);
    
        b->speed.x +=( color1.r-0.5)*0.5;
        b->speed.y +=( color1.g-0.5)*0.5;
        b->speed.z +=( color1.b-0.5)*0.1;
        b->speed*=0.99;
        
        b->position+=b->speed;
        
        if(b->position.z>-20){
            b->position.z=-20;
            

        }
        if(b->position.z<-99){
            b->position.z=-99;
            
            
        }
        
        
        
        b->position =mod(b->position,vec3( fullSize, fullSize,-100));
       // b->position.z =color1.b*-100-10;
        vec2 posN =vec2(b->position.x,b->position.y)/vec2(fullSize,fullSize)* vec2(numTiles,numTiles);
        int posX =(int)floor(posN.x)%10;
        int posY =(int)floor(posN.y)%10;
      
        auto t =tiles[posX +posY* numTiles];
       
        b->positionDraw.x =b->position.x-t->posXWorld-tileSize/2;
        b->positionDraw.y =b->position.y-t->posYWorld-tileSize/2;
        b->positionDraw.z =b->position.z;
        
        t->butterflys.push_back(b);
    }
  

}
void ButterflyRenderer::draw(PlayerLevel &playerLevel,double currentTime,ci::vec3 lightPos)
{
   
    gl::pushMatrices();
    gl::translate( playerLevel.player->playerWorldOffset);
    
     mGlsl->bind();
    mGlsl->uniform("time", (float)currentTime*15);
  mGlsl->uniform("lightPos",lightPos);
  
    for(auto tile : playerLevel.playerTiles)
    {
        tile->drawButterflys();
    }
   
    
    gl::popMatrices();



}
