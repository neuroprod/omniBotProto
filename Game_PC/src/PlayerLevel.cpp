//
//  PlayerLevel.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 17/06/2017.
//
//

#include "PlayerLevel.hpp"

using namespace std;
using namespace ci;
using namespace ci::app;

void PlayerLevel::setup()
{
    for(auto tile :tiles)
    {
        auto playerTile = PlayerTile::create();
        playerTile->tile = tile;
        playerTile->setup();
        playerTiles.push_back(playerTile);
    
    }



}


void PlayerLevel::update(glm::vec2 deviderLineP1,glm::vec2 deviderLineP2)
{
    int playerTilePosXTemp =player->playerWorldPos.x/tileSize;
    int playerTilePosYTemp =player->playerWorldPos.y/tileSize;
    if(playerTilePosXTemp !=playerTilePosX || playerTilePosYTemp !=playerTilePosY || devPos !=deviderLineP1 )
    {
        playerTilePosX =playerTilePosXTemp;
        playerTilePosY =playerTilePosYTemp;
        devPos=deviderLineP1 ;
        for(auto tile : playerTiles)
        {
            tile->update(playerTilePosX,playerTilePosY,numTiles,player->playerWorldOffset,player->circleCenter,deviderLineP1, deviderLineP2);
          
        }
 
    }
    for(auto tile : playerTiles)
    {
        if( tile->visible)
        {
             tile->updatePlayer(player->playerWorldPos);
            tile->tile->visible =true;
        }
        
    }

    
}
