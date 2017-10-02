//
//  Player.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 16/05/2017.
//
//

#include "Player.hpp"

#define sin120 0.866025
#define cos120 -0.5
#define sinM120 -0.866025
#define cosM120 -0.5



using namespace std;
using namespace ci;
using namespace ci::app;

Player::Player()
{

}

Player::~Player()
{

}

PlayerRef  Player::create()
{
    return make_shared<Player>();
}

void Player::setup()
{
   overlay =gl::Texture::create( loadImage(getAssetPath("insideOverlay.png")),gl::Texture::Format().loadTopDown());
}


void Player::updateDebug(double elapsed)
{
    
    robotDir.x =currentDirection.x;
    robotDir.y =currentDirection.y;
    robotDir = glm::normalize(robotDir);
    
    float robotAngle=atan2(robotDir.y, robotDir.x);
    
    float robotAngleAdj=3.1415/2- robotAngle-3.1415;
    
    float dirAngleComp=0;
    robotDirRot.x =controler.x * cos(robotAngleAdj) -controler.y * sin(robotAngleAdj);
    robotDirRot.y=controler.x * sin(robotAngleAdj) +controler.y * cos(robotAngleAdj);
    
    //console()<<name <<glm::length(controler)<<endl;
    float vecX = controler.x;
    float vecY = -controler.y;
    
    
    vecX = round( robotDirRot.x*10)/10;
    vecY = round(-robotDirRot.y*10)/10;
    
    motorSpeed.x =vecX -dirAngleComp;
    motorSpeed.y =vecX*cosM120-vecY *sinM120-dirAngleComp;
    motorSpeed.z =vecX*cos120-vecY *sin120-dirAngleComp;
    
    string com=name+"0:" +to_string(motorSpeed.x*2) +":"+to_string(motorSpeed.y*2)  + ":"+ to_string(motorSpeed.z*2) +"\n";
    
    sendCount++;
    if(sendCount>2){
        
        if(com !=command)
        {
           
            command =com;
            hasNewCommand =true;
            sendCount=0;
            
        }
    }

}
void Player::update(double elapsed)
{
    if(!hasPosSet ) return;
    
    playerWorldPos+=controler;
    if(playerWorldPos.x<0)
    {
        playerWorldPos.x+= levelSize;
        
    }  if(playerWorldPos.y<0)
    {
        playerWorldPos.y+= levelSize;
        
    }
    if(playerWorldPos.x>levelSize)
    {
        playerWorldPos.x-= levelSize;
        
    }  if(playerWorldPos.y>levelSize)
    {
        playerWorldPos.y-= levelSize;
        
    }
    
    
    if(controler.x !=0 || controler.y !=0)
    {
        
        
        vec2 LineP1 =playerViewPos;
        vec2 LineP2 =playerViewPos+controler ;
        vec2 LocalP1 = LineP1-circleCenter;
        vec2 LocalP2 = LineP2-circleCenter;
        // Precalculate this value. We use it often
        vec2 P2MinusP1 = LocalP2-LocalP1;
        
        
        
        
        float a = (P2MinusP1.x) * (P2MinusP1.x) + (P2MinusP1.y) * (P2MinusP1.y);
        float b = 2 * ((P2MinusP1.x * LocalP1.x) + (P2MinusP1.y * LocalP1.y));
        float c = (LocalP1.x * LocalP1.x) + (LocalP1.y * LocalP1.y) - ((720/2)*(720/2));//720 =radius
        float delta = b * b - (4 * a * c);
        if (delta < 0){
            
            // No intersection
            console()<<"ERROR? noItersection"<<endl;
        }
        else if (delta == 0){
            
            console()<<"ERROR? 1Itersection"<<endl;
            // One intersection
            //float  u = -b / (2 * a);
            // return LineP1 + (u * P2MinusP1)
            
        }
        else if (delta > 0){
            /// console()<<"2intersection"<<endl;
            // Two intersections
            float SquareRootDelta = sqrt(delta);
            
            float  u1 = (-b + SquareRootDelta) / (2 * a);
            float  u2 = (-b - SquareRootDelta) / (2 * a);
            
            vec2 intersection1  = LineP1 + (u1 * P2MinusP1);
            
            
            
            if(glm::distance2(intersection1, LineP1 )>glm::distance2(intersection1, LineP2 ))
            {
                intersection =intersection1;
                
            }else
            {
                intersection = LineP1 + (u2 * P2MinusP1);
                
            }
            
        }
        
        float distance  = glm::distance(intersection, playerViewPos);
        float factor = (distance-100)/150;
        
        factor = glm::clamp(factor, 0.f, 1.f);
        
        
        
        playerViewPos+=controler*factor;
    }
    vec2 centerVec =playerViewPos-circleCenter;
    float centerVecLength =glm::length(centerVec);
    if(centerVecLength>250)
    {
        
        centerVec = glm::normalize( centerVec );
       // playerViewPos+=centerVec *(250- centerVecLength);
        
    }
    
    
    
    
}
void Player::updateWorldOffset()
{
    if(!hasPosSet) return;
    
    
    playerWorldOffset =playerViewPos-playerWorldPos;
    
    
    //send move to robots
    
    //robot move vector
    
    vec2 move = vec2(currentPosition.x,currentPosition.y)-playerViewPos;
    
    
    //robot rotation
    
    robotDir.x =currentDirection.x;
    robotDir.y =currentDirection.y;
    robotDir = glm::normalize(robotDir);
    float robotAngle=atan2(robotDir.y, robotDir.x);
    float robotAngleAdj=3.1415/2- robotAngle-3.1415;
    float dirAngleComp=0;
    
    
    //adjust move dir with rotation
    robotDirRot.x =move.x * cos(robotAngleAdj) -move.y * sin(robotAngleAdj);
    robotDirRot.y=move.x * sin(robotAngleAdj) +move.y * cos(robotAngleAdj);
    
    
    float vecX = move.x;
    float vecY = -move.y;
    
    
    vecX = round(-robotDirRot.x*10)/10;
    vecY = round(robotDirRot.y*10)/10;
    
    
    motorSpeed.x =vecX -dirAngleComp;
    motorSpeed.y =vecX*cosM120-vecY *sinM120-dirAngleComp;
    motorSpeed.z =vecX*cos120-vecY *sin120-dirAngleComp;
    
    string com=name+"0:" +to_string(motorSpeed.x) +":"+to_string(motorSpeed.y)  + ":"+ to_string(motorSpeed.z) +"\n";
    
    sendCount++;
    if(sendCount>2){
        
        if(com !=command)
        {
            command =com;
            hasNewCommand =true;
            sendCount=0;
        }
    }

    
    
    

}
void Player::draw()
{


    
     gl::color(0.8,0.8,0.8);
    gl::drawStrokedCircle(playerViewPos, robotSize);
   // gl::draw(overlay,Rectf(playerViewPos.x-robotSize,playerViewPos.y-robotSize,playerViewPos.x+robotSize,playerViewPos.y+robotSize));
    
    
     gl::drawSolidCircle(playerViewPos, robotSize);

}





void Player::setRobotPosition(glm::vec4 _currentPosition,glm::vec4 _currentDirection,double elapsed){

    hasPosSet  =true;
    cameraSet =true;
    cameraPositionSpeed =_currentPosition-currentPosition;
    cameraPositionSpeed/=elapsed;
    
    
    console()<<name<<glm::length(cameraPositionSpeed)<<endl;
    
    
    playerViewPos.x =_currentPosition.x;
    playerViewPos.y =_currentPosition.y;
    currentPosition = _currentPosition;
    
    
    currentDirection =_currentDirection;
    
    
    moveSpeed2D.x =cameraPositionSpeed.x;
    moveSpeed2D.y = cameraPositionSpeed.y;
    
    drawPosition2D.x =currentPosition.x;
    drawPosition2D.y =currentPosition.y;
    
    
   // drawPosition2D +=moveSpeed2D*((float)elapsed*moveOffsetStart);

}



//////////////////////
//////////////////////
void Player::setUseCamera(bool _useCamera)
{
    useCamera = _useCamera;
    
    
    
    
    
    if(name =="1:")
    {
        // setPosition(vec4(350,350,0,1),vec4(0,1,0,1),0.1);
        playerWorldPos.x = 0;
        playerWorldPos.y = 0;
        
        playerViewPos.x = 450;
        playerViewPos.y = 350;
        
        
        
    }else
    {
        
        
        
        playerWorldPos.x = 500;
        playerWorldPos.y = 500;
        
        playerViewPos.x = 1000;
        playerViewPos.y = 350;
        
        
        
        
        
    }

    
    
    
    
    if(!useCamera)
    {
        if(name =="1:")
        {
           // setPosition(vec4(350,350,0,1),vec4(0,1,0,1),0.1);
            playerWorldPos.x = 0;
            playerWorldPos.y = 0;
            
            playerViewPos.x = 450;
            playerViewPos.y = 350;
            
            
            
        }else
        {
           
            
            
            playerWorldPos.x = 500;
            playerWorldPos.y = 500;
            
            playerViewPos.x = 1000;
            playerViewPos.y = 350;
            
            
          
            
            
        }
        
        vec2 centerVec =playerViewPos-circleCenter;
        float centerVecLength =glm::length(centerVec);
        if(centerVecLength>250)
        {
            
            centerVec = glm::normalize( centerVec );
            playerViewPos+=centerVec *(250- centerVecLength);
            
        }
        
        playerWorldOffset =playerViewPos-playerWorldPos;

    }
    
};



void Player::parseControles(vector<std::string> substrings)
{
    float x =(stof(substrings[1])-512)*-1;
    float y =stof(substrings[2]) -512;
    
    if(x<0)
    {
        if(x>-20)
        {
            x=0;
        }else
        {
            x+=20;
            x/=(512-20);
        }
    }
    else if(x>0)
    {
        if(x<20)
        {
            x=0;
        }else
        {
            x-=20;
            x/=(512-20);
        }
    }
    
    
    
    if(y<0)
    {
        if(y>-20)
        {
            y=0;
        }else
        {
            y+=20;
            y/=(512-20);
        }
    }
    else if(y>0)
    {
        if(y<20)
        {
            y=0;
        }else
        {
            y-=20;
            y/=(512-20);
        }
    }
    
    
    
    int btn =stoi(substrings[3]);
    controler.x =x;
    controler.y =y;
    btnDown = btn ;
    
    
    
}

void Player::drawDebug(ci::Camera cam)
{
    float yPos = 100;
    if(name=="2:")yPos+=200;
    
    if(btnDown)
    {
        gl::color(1,1,0);
    }else
    {
        gl::color(1,1,1);
    }
    gl::drawLine(vec2(1280-100,yPos), vec2(1280-100,yPos)+ controler*80.f);
    gl::color(1,0,0);
    gl::drawLine(vec2(1280-100,yPos), vec2(1280-100,yPos)+ robotDirRot*80.f);
    
    
    gl::color(1,0,0);
    gl::drawLine(vec2(1280-60,yPos), vec2(1280-60,yPos-motorSpeed.x*100));
    gl::color(0,1,0);
    gl::drawLine(vec2(1280-40,yPos), vec2(1280-40,yPos-motorSpeed.y*100));
    gl::color(0,0,1);
    gl::drawLine(vec2(1280-20,yPos), vec2(1280-20,yPos-motorSpeed.z*100));
    
    if(cameraSet){
        gl::color(1,1,1);
    }
    else
    {
        
        gl::color(1,1,1);
    }
    
    gl::drawStrokedCircle(drawPosition2D, robotSize);
    gl::color(1,1,1);
    
    
    
    gl::drawLine(vec2(currentPosition.x,currentPosition.y+100), vec2(currentPosition.x,currentPosition.y-100));
    gl::drawLine(vec2(currentPosition.x-100,currentPosition.y), vec2(currentPosition.x+100,currentPosition.y));
    gl::color(1,1,1);
    
    gl::drawLine(vec2(currentPosition.x,currentPosition.y), vec2(currentPosition.x,currentPosition.y)+ robotDir*80.f);
    string posString = "x: "+to_string((int)currentPosition.x)+" y: "+to_string((int)currentPosition.y);
    
    gl::drawString(posString ,vec2(currentPosition.x+10,currentPosition.y-70));
    gl::drawString(name ,vec2(currentPosition.x,currentPosition.y));
    
    
    gl::enableDepth();
    gl::pushMatrices();
    gl::setMatrices(cam);
    
    
    
    
    gl::color(0.5,0.5,0.5);
    gl::enableAlphaBlending();
    gl::translate(drawPosition2DFloor.x, drawPosition2DFloor.y, 0);
    gl::rotate(3.1415/2,vec3(1,0,0));
    gl::setWireframeEnabled(true);
    gl::color(Color::gray(0.2));
    // mBatch->draw();
    gl::setWireframeEnabled(false);
    gl::popMatrices();
    gl::enableDepth(false);
    gl::enableAlphaBlending(false);
    gl::color(1,1,1);
    gl::drawStrokedCircle(drawPosition2DFloor, robotSize);
    gl::drawLine(vec2(drawPosition2DFloor.x-20, drawPosition2DFloor.y+robotSize), vec2(drawPosition2DFloor.x+20, drawPosition2DFloor.y+robotSize));
    gl::drawLine(vec2(drawPosition2DFloor.x+robotSize, drawPosition2DFloor.y-50), vec2(drawPosition2DFloor.x+robotSize, drawPosition2DFloor.y+50));
    
    gl::drawLine(vec2(drawPosition2DFloor.x-robotSize, drawPosition2DFloor.y-50), vec2(drawPosition2DFloor.x-robotSize, drawPosition2DFloor.y+50));
    
}

