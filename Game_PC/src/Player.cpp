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
    
   
  
    mBatch = gl::Batch::create( geom::Cylinder().height(100).radius(50).subdivisionsHeight(1).subdivisionsAxis(20),gl::getStockShader(gl::ShaderDef().color())  );
}

void Player::setUseCamera(bool _useCamera)
{
    useCamera = _useCamera;
    if(!useCamera)
    {
        if(name =="1:")
        {
            setPosition(vec4(350,350,0,1),vec4(0,1,0,1),0.1);
        
        }else
        {
            setPosition(vec4(1000,350,0,1),vec4(0,1,0,1),0.1);
        
        }
    
    
    }

};


void Player::update(double elapsed)
{
    if(!useCamera)
    {
        setPosition( currentPosition+vec4(controler.x *4,controler.y*4,0,0),vec4(0,1,0,1),elapsed);
        
        
    }

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

    string com=name+"0:" +to_string(motorSpeed.x*2) +":"+to_string(motorSpeed.y*2)  + ":"+ to_string(motorSpeed.z*2) +"\n";
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
    
   
    if(!cameraSet){
    drawPosition2D +=moveSpeed2D*((float)elapsed*moveOffset);
    }
     cameraSet =false;
    
}
void Player::draw()
{


    gl::color(0.3,0.3,0.3);
    gl::drawSolidCircle(drawPosition2D, robotSize);
    gl::drawSolidCircle(drawPosition2DFloor, robotSize-1);

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

void Player::setPosition(glm::vec4 _currentPosition,glm::vec4 _currentDirection,double elapsed){

    cameraSet =true;
    cameraPositionSpeed =_currentPosition-currentPosition;
    
    cameraPositionSpeed/=elapsed;
    currentPosition = _currentPosition;
    currentDirection =_currentDirection;
    moveSpeed2D.x =cameraPositionSpeed.x;
    moveSpeed2D.y = cameraPositionSpeed.y;
    
    drawPosition2D.x =currentPosition.x;
    drawPosition2D.y =currentPosition.y;
    drawPosition2D +=moveSpeed2D*((float)elapsed*moveOffsetStart);

}

