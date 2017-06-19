//
//  PlayerLevel.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 17/06/2017.
//
//

#ifndef PlayerLevel_hpp
#define PlayerLevel_hpp

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Player.hpp"
#include "LevelTile.hpp"
#include "PlayerTile.hpp"



class PlayerLevel
{
public:
    PlayerLevel(){}
    void setup();
    void update(glm::vec2 deviderLineP1,glm::vec2 deviderLineP2);
    void draw();
    
    PlayerRef player;
    
    float tileSize ;
    int numTiles;

    
    std::vector<PlayerTileRef> playerTiles;
    std::vector<LevelTileRef> tiles;
    
    int playerTilePosX =-10;
    int playerTilePosY =-10;
    glm::vec2 devPos;
    
};
#endif /* PlayerLevel_hpp */
