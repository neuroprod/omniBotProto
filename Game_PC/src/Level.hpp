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
#include "FloorMapping.hpp"
#include "ObjectMap.hpp"

#include "GrassRenderer.hpp"
#include "FlowerRenderer.hpp"
#include "ButterflyRenderer.hpp"


class Level
{

public:
    Level(){};
    
    void setup(PlayerRef player1,PlayerRef player2);
    
    void updatePlayerPositions(glm::vec2 deviderLineP1,glm::vec2 deviderLineP2);

    void update();
    
    
    
    void draw(int playerID,ci::gl::FboRef shadowFBO,ci::mat4 &shadowMatrix,ci::vec3 lightPos);
    void drawShadow(int playerID);
    float tileSize =256;
    int numTiles=10;
    float levelSize;
    double currentTime;
    
    PlayerLevel player1Level;
    PlayerLevel player2Level;
    
    FloorMapping floorMap;
    ObjectMap objMap;
    GrassRenderer grassRenderer;
    FlowerRenderer flowerRenderer;
    ButterflyRenderer butterflyRenderer;
    std::vector<LevelTileRef> tiles;

};
#endif /* Level_hpp */
