//
//  Level.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 16/06/2017.
//
//

#ifndef Level_hpp
#define Level_hpp

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "LevelTile.hpp"
#include "Player.hpp"
#include "PlayerLevel.hpp"

class Level
{

public:
    Level(){};
    
    void setup(PlayerRef player1,PlayerRef player2);
    
    void updatePlayerPositions(glm::vec2 deviderLineP1,glm::vec2 deviderLineP2);

    void update();
    
    
    
    void draw(int playerID);

    float tileSize =200;
    int numTiles=30;
    float levelSize;
    
    
    PlayerLevel player1Level;
    PlayerLevel player2Level;
    
    
    std::vector<LevelTileRef> tiles;

};
#endif /* Level_hpp */
