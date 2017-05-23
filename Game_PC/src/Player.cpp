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

}
void Player::update()
{
    robotDir.x =currentDirection.x;
    robotDir.y =currentDirection.y;
    robotDir = glm::normalize(robotDir);
    
    float robotAngle=atan2(robotDir.y, robotDir.x);
    
    float robotAngleAdj=3.1415/2- robotAngle-3.1415;
    //console()<< "robot angle: "<< (robotAngle/3.1415*180)<<endl;
    
    //controler = glm::normalize(controler);
    float dirAngleComp=0;
    robotDirRot.x =controler.x * cos(robotAngleAdj) -controler.y * sin(robotAngleAdj);
    robotDirRot.y=controler.x * sin(robotAngleAdj) +controler.y * cos(robotAngleAdj);
    
   // float vecX = controler.y * cos(robotAngle) +controler.y * sin(robotAngle);
   // float vecY = controler.x * sin(robotAngle) -controler.y * cos(robotAngle);
    
    float vecX = controler.x;
    float vecY = -controler.y;
    
    
    vecX = round( robotDirRot.x*10)/10;
    vecY = round(-robotDirRot.y*10)/10;
    
    motorSpeed.x =vecX -dirAngleComp;
    motorSpeed.y =vecX*cosM120-vecY *sinM120-dirAngleComp;
    motorSpeed.z =vecX*cos120-vecY *sin120-dirAngleComp;

    string com="0:" +to_string(motorSpeed.x*2) +":"+to_string(motorSpeed.y*2)  + ":"+ to_string(motorSpeed.z*2) +"\n";
    count++;
    if(count>2){
    
    if(com !=command)
    {
        command =com;
        hasNewCommand =true;
        count=0;
       // console()<<command<<endl;
    }else
    {
        //console()<<"-";
    
    }
    
    }
    
    
}
void Player::draw()
{


    gl::color(0.5,0.5,0.5);
    gl::drawSolidCircle(drawPosition2D, robotSize);

}
void Player::drawDebug()
{

    if(btnDown)
    {
        gl::color(1,1,0);
    }else
    {
        gl::color(1,1,1);
    }
    gl::drawLine(vec2(1280-100,100), vec2(1280-100,100)+ controler*80.f);
    gl::color(1,0,0);
    gl::drawLine(vec2(1280-100,100), vec2(1280-100,100)+ robotDirRot*80.f);
    
    
    gl::color(1,0,0);
    gl::drawLine(vec2(1280-60,100), vec2(1280-60,100-motorSpeed.x*100));
    gl::color(0,1,0);
    gl::drawLine(vec2(1280-40,100), vec2(1280-40,100-motorSpeed.y*100));
    gl::color(0,0,1);
    gl::drawLine(vec2(1280-20,100), vec2(1280-20,100-motorSpeed.z*100));
    gl::color(1,1,1);
    gl::drawStrokedCircle(vec2(currentPosition.x,currentPosition.y), robotSize);
    
     gl::drawLine(vec2(currentPosition.x,currentPosition.y+100), vec2(currentPosition.x,currentPosition.y-100));
     gl::drawLine(vec2(currentPosition.x-100,currentPosition.y), vec2(currentPosition.x+100,currentPosition.y));
    gl::color(1,1,1);
    
    gl::drawLine(vec2(currentPosition.x,currentPosition.y), vec2(currentPosition.x,currentPosition.y)+ robotDir*80.f);
    string posString = "x: "+to_string((int)currentPosition.x)+" y: "+to_string((int)currentPosition.y);
    
    gl::drawString(posString ,vec2(currentPosition.x+10,currentPosition.y-70));
    
    gl::begin( GL_POINTS);

     gl::color(1,1,1);
    
    for(int i=0;i<20;i++)
    {
        gl::vertex( vec2(currentPosition.x-20+i*2,currentPosition.y+70) );
        gl::vertex( vec2(currentPosition.x+70,currentPosition.y-20+i*2 ));
    }
    gl::end();
    gl::color(1,1,1);
}



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

void Player::setPosition(glm::vec4 _currentPosition,glm::vec4 _currentDirection){

    
    cameraPositionSpeed =currentPosition-_currentPosition;
    currentPosition = _currentPosition;
    currentDirection =_currentDirection;
    
    float factor =1;
    drawPosition2D.x =currentPosition.x+cameraPositionSpeed.x*factor;
    drawPosition2D.y =currentPosition.y+cameraPositionSpeed.y*factor;
    moveSpeed2D.x =cameraPositionSpeed.x;
    moveSpeed2D.y = cameraPositionSpeed.y;

}

