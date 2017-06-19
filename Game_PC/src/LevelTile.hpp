//
//  LevelTile.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 16/06/2017.
//
//

#ifndef LevelTile_hpp
#define LevelTile_hpp

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

class LevelTile;
typedef std::shared_ptr<LevelTile> LevelTileRef;


class LevelTile
{

public:
    LevelTile(){};
    
    static LevelTileRef create();
    
    void setup(int x,int y, float size);
    void draw();

    int xR;
    int yR;
    float size;
    
    
    float posXWorld;
    float posYWorld;
    
    ci::Color testColor;
    
};
#endif /* LevelTile_hpp */
