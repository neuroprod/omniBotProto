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
    
    
    ci::gl::TextureRef overlay;
    
    
    void setup();
    void draw();
    void drawDebug(ci::Camera cam);
    void update(double elapsed);
    void updateDebug(double elapsed);
    void updateWorldOffset();
    void setUseCamera(bool _useCamera);
    bool useCamera =true;
    
   
    
    void parseControles( std::vector<std::string> substrings);
    void setRobotPosition(glm::vec4 _currentPosition,glm::vec4 _currentDirection,double elapsed);

    std::string name;
    int id;
    
    
    float levelSize;
    
    
    bool hasPosSet  =false;
    
    glm::vec2 robotRotation;//keep robot strait/resolve move
    glm::vec2 robotCurrentPos;//current calibrated camera pos
    
    glm::vec2 robotTargetMove;
    glm::vec2 robotTargetPos;
    
    
    
    glm::vec2 playerWorldPos;
    glm::vec2 playerViewPos;
    glm::vec2 playerWorldOffset;

    
    
    glm::vec3 motorSpeed;
    glm::vec2 circleCenter;
 
    
    
    glm::vec4 currentPosition;
    glm::vec4 currentDirection;
    glm::vec2 robotDir;
    
    
    glm::vec2 controler;
    bool btnDown;
    
    bool hasNewCommand=false;
    std::string command ="";
    
    float robotSize=52;
    bool cameraSet =true;
    glm::vec4 cameraPositionSpeed;
    glm::vec2 robotDirRot;
    
    
    glm::vec2 drawPosition2D;
     glm::vec2 drawPosition2DFloor;
    glm::vec2 moveSpeed2D;
    
   // float moveOffset =0.0;
    // float moveOffsetStart =0.0;
    
    
    glm::vec2 intersection;
    
    
    
    int sendCount =0;
   
    
};
#endif /* Player_hpp */
