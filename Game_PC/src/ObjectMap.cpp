//
//  ObjectMap.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 23/06/2017.
//
//

#include "ObjectMap.hpp"
#include "glm/gtc/random.hpp"
#include "cinder/Perlin.h"

using namespace ci;
using namespace ci::app;
using namespace std;



void ObjectMap::load()
{
    
    
    dataSurface =  loadImage(getAssetPath("levelObj.png"));
    grassGradient =loadImage(getAssetPath("grassGradient.png"));
   
    
}


void ObjectMap::setTileFloorMesh(LevelTileRef tile,int numTiles)
{
    Perlin perlin;
    perlin.setSeed(20);
      int size = tile->size;
    int size2 = size/2;
    int xPos = tile->xR*size;
    int yPos = tile->yR*size;
    
    
    
       
    GrassPatchRef grassPatch = GrassPatch::create();
    
    bool hasGrass =false;

    for(int y=0;y < size ; y++)
    {
        int lY = y+yPos;
        
        for(int x=0;x < size ; x++)
        {
            int lX = x+xPos;
            
            
            ColorA col= dataSurface.getPixel(vec2(lX,lY));
            
            if(col.r ==0 && col.g==1 && col.b==0)
            {
                if(glm::linearRand(0.f,1.f)>0.8)
                {
                    hasGrass =true;
               
                    grassPatch->addGrass(x,y,lX,lY,size2,  grassGradient);
                    
                    
                }
            }
            
        }
    }
   
    if(hasGrass)
    {
       
        grassPatch->setup();
        tile->grassPatch = grassPatch;
    }
}
