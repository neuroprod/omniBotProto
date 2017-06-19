//
//  Level.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 16/06/2017.
//
//

#include "Level.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;
void Level::setup(PlayerRef player1,PlayerRef player2)
{
   
    levelSize = tileSize*numTiles;
    
    for(int y=0;y<numTiles;y++)
    {
        for(int x=0;x<numTiles;x++)
        {
            LevelTileRef tile = LevelTile::create();
            tile->setup(x,y,tileSize);
            
            tiles.push_back(tile);
        }
    
    }
    

    player1Level.tiles =tiles;
    player1Level.player = player1;
    player1Level.tileSize =tileSize;
    player1Level.numTiles=numTiles;

    player1Level.setup();
    
    
    
    player2Level.tiles =tiles;
    player2Level.player = player2;
    player2Level.tileSize =tileSize;
    player2Level.numTiles=numTiles;
    player2Level.setup();
}

void Level::updatePlayerPositions(glm::vec2 deviderLineP1,glm::vec2 deviderLineP2)
{

    player1Level.update(deviderLineP1,deviderLineP2);
    player2Level.update(deviderLineP2,deviderLineP1);

}
void Level::update()
{
//resolve tile physics;
}

void Level::draw(int playerID)
{
    if(playerID==0)
    {
        player1Level.draw();
    }
    else
    {
        player2Level.draw();
    }
}
