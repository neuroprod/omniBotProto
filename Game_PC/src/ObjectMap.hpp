//
//  ObjectMap.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 23/06/2017.
//
//

#ifndef ObjectMap_hpp
#define ObjectMap_hpp


#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "LevelTile.hpp"


class ObjectMap{

public:
    ObjectMap(){};
    ci::Surface dataSurface;
    ci::Surface grassGradient;
    void load();
    void setTileFloorMesh(LevelTileRef tile,int numTiles);
    
};
#endif /* ObjectMap_hpp */
