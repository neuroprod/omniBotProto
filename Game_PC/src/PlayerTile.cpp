//
//  PlayerTile.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 17/06/2017.
//
//

#include "PlayerTile.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;



PlayerTileRef PlayerTile::create()
{
    return  make_shared<PlayerTile>();
}
void PlayerTile::setup()
{
    xR = tile->xR;
    yR =tile->yR;
    size = tile->size;
    float sizeDia2=  sqrt(powf(size,2.0f) *2)/2;
    radius2 =    powf((360+sizeDia2),2.f);
}

void PlayerTile::updatePlayer(vec2 playerPos)
{
    vec2 lPos = playerPos-localTrans;
    if(glm::length(lPos)<200){
        if(tile->grassPatch)
        {
            tile->grassPatch->updatePlayer(lPos);
    
        }
    }
}

void PlayerTile::update(int playerTilePosX,int playerTilePosY,int numTiles,glm::vec2 offset,glm::vec2 circleCenter,glm::vec2 deviderLineP1,glm::vec2 deviderLineP2)
{
    int numTiles2 = numTiles/2;
  
    
    int xOf =xR-playerTilePosX;
    int xL = xR;
    if(xOf>numTiles2)xL-=numTiles;
    else if(xOf<-numTiles2)xL+=numTiles;
    
    int yOf =yR-playerTilePosY;
    int yL = yR;
    if(yOf>numTiles2)yL-=numTiles;
    else if(yOf<-numTiles2)yL+=numTiles;
   
    
    localTrans.x =xL*size;
    localTrans.y = yL*size;
    
    visible =true;
    vec2 viewSpace = offset +localTrans;
    if( glm::distance2( circleCenter,viewSpace )>radius2) //outside main circle
    {
        
        visible=false;
    }
    else
    {
        //size = (x1 - x0)(y2 - y0) - (x2 - x0)(y1 - y0)
        
        float side= (deviderLineP2.x  - deviderLineP1.x)*(viewSpace.y - deviderLineP1.y) - (viewSpace.x - deviderLineP1.x) *(deviderLineP2.y - deviderLineP1.y);
        if(side>0)
        {
        
            const float l2 = glm::distance2(deviderLineP1, deviderLineP2);  // i.e. |w-v|^2 -  avoid a sqrt
            
            const float t = max(0.f, min(1.f, glm::dot(viewSpace  - deviderLineP1, deviderLineP2 - deviderLineP1) / l2));
            const vec2 projection = deviderLineP1 + t * (deviderLineP2 - deviderLineP1);  // Projection falls on the segment
            float dist = glm::distance(viewSpace, projection);
            if(dist>size)
            {
                visible=false;
            
            }
            
        }
       
    }
    
        
   
}

void PlayerTile::drawGrass()
{
    if(!visible ||  !tile->grassPatch)
    {
        return;
    }
    gl::pushMatrices();
    gl::translate(localTrans);
    tile->grassPatch->draw();
    gl::popMatrices();
    
}
void PlayerTile::drawFloor()
{
    if(!visible)
    {
        return;
    }
    gl::pushMatrices();
    gl::translate(localTrans);
    gl::draw(tile->meshFloor);
    gl::popMatrices();
    
}

void PlayerTile::drawCube()
{
    //translateLocakl
    if(!visible)
    {
        return;
    }
    gl::pushMatrices();
   
    gl::translate(localTrans);
    rotCount+=0.01;
    gl::rotate(rotCount);
    gl::drawColorCube(vec3(0,0,-40+ -sin(rotCount)*20), vec3(40,40,40));
    ////
    
    gl::popMatrices();
    
}
