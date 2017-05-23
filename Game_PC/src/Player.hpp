//
//  Player.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 16/05/2017.
//
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "cinder/app/App.h"

class Player;
typedef std::shared_ptr<Player> PlayerRef;

class Player
{
    
public:
    Player();
    ~Player();
    
    static PlayerRef create();
    
    void setup();
    void draw();
    void drawDebug();
    void update();
    
    
    
    void parseControles( std::vector<std::string> substrings);
    void setPosition(glm::vec4 _currentPosition,glm::vec4 _currentDirection);

    glm::vec3 motorSpeed;
    
    int count=0;
    glm::vec4 currentPosition;
    glm::vec4 currentDirection;
    glm::vec2 robotDir;
    
    
    glm::vec2 controler;
    bool btnDown;
    
    bool hasNewCommand=false;
    std::string command ="";
    
    float robotSize;
    
    glm::vec4 cameraPositionSpeed;
    glm::vec2 robotDirRot;
    
    
    glm::vec2 drawPosition2D;
    glm::vec2 moveSpeed2D;
    
    
};
#endif /* Player_hpp */
