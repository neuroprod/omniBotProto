//
//  PlayerTile.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 17/06/2017.
//
//

#ifndef PlayerTile_hpp
#define PlayerTile_hpp


#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "LevelTile.hpp"
class PlayerTile;
typedef std::shared_ptr<PlayerTile> PlayerTileRef;


class PlayerTile
{
    
public:
    PlayerTile(){};
    
    static PlayerTileRef create();
    LevelTileRef tile;
    void setup();
    void update(int playerTilePosX,int playerTilePosY,int numTiles,glm::vec2 offset,glm::vec2 circleCenter,glm::vec2 deviderLineP1,glm::vec2 deviderLineP2);
    void draw();
    
    int xR;
    int yR;
    float size;
    glm::vec2 localTrans;
    
    
    std::string testString;
    bool visible;
    float radius2;
};



#endif /* PlayerTile_hpp */
