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
    
    
    dataSurface =  loadImage(getAssetPath("noise.jpg"));
    grassGradient =loadImage(getAssetPath("grassGradient.png"));
   
    perlin.setSeed(20);
}


void ObjectMap::setTileFloorMesh(LevelTileRef tile,int numTiles)
{
    
    int size = tile->size;
    int size2 = size/2;
    int xPos = tile->xR*size;
    int yPos = tile->yR*size;
    
    
    
       
    GrassPatchRef grassPatch = GrassPatch::create();
    
    bool hasGrass =false;
    float fullSize = size*numTiles;
    for(int y=0;y < size ; y++)
    {
        int lY = y+yPos;
        
        for(int x=0;x < size ; x++)
        {
            int lX = x+xPos;
            
            vec2 posNormal =vec2((float)lX/fullSize,(float)lY/fullSize);
            ColorA color1= dataSurface.getPixel(posNormal *500.f);
            ColorA color2= dataSurface.getPixel(mod((posNormal *2000.f),vec2(500.f,500.f)));
            ColorA color3= dataSurface.getPixel(mod((posNormal *8000.f),vec2(500.f,500.f)));
            
          
            if( color1.r>0.4 && color2.r>0.4 && color3.r>0.4 )
            {
                if(glm::linearRand(0.f,1.f)>0.990)
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
