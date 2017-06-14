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
    void drawDebug(ci::Camera cam);
    void update(double elapsed);
    void setUseCamera(bool _useCamera);
    
    bool useCamera =true;
    
    std::string name;
    
    void parseControles( std::vector<std::string> substrings);
    void setPosition(glm::vec4 _currentPosition,glm::vec4 _currentDirection,double elapsed);

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
    bool cameraSet =true;
    glm::vec4 cameraPositionSpeed;
    glm::vec2 robotDirRot;
    
    
    glm::vec2 drawPosition2D;
     glm::vec2 drawPosition2DFloor;
    glm::vec2 moveSpeed2D;
    
    float moveOffset =0.0;
    float moveOffsetStart =0.0;
    
    cinder::gl::BatchRef		mBatch;
    
   
    
};
#endif /* Player_hpp */
