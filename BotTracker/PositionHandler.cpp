//
// Created by pi on 22/03/17.
//
#include <glm/glm.hpp>
#include "PositionHandler.h"


#define sin120 0.866025
#define cos120 -0.5
#define sinM120 -0.866025
#define cosM120 -0.5
using namespace std;
using namespace glm;
void PositionHandler::setup()
{
    pCenter =vec2(0,0);
    pDir =vec2(1,0);
    drawer.setup();
}

void PositionHandler::setPoints(std::vector<glm::vec2> rawPoints)
{
    if( rawPoints.size()==3)
    {

        glm::vec2 p1 = rawPoints[0];
        glm::vec2 p2 = rawPoints[1];
        glm::vec2 p3 = rawPoints[2];

       float d1 = glm::distance(p1,p2);
        float d2 =  glm::distance(p2,p3);
        float d3 =glm::distance(p1,p3);
       // cout << p1.x <<" "<< p2.x <<" "<< p3.x <<" "<<endl;
       // cout << d1 <<" "<< d2 <<" "<< d3 <<" "<<endl;
        if(d1>d2 && d1>d3)
        {
            pCenter =mix (p1,p2,0.5);
            pDir = p3;
        }else if(d2>d1 && d2>d3)
        {
            pCenter =mix (p3,p2,0.5);
            pDir = p1;
        }else
        {
            pCenter =mix (p1,p3,0.5);
            pDir = p2;

        }
        nDir = glm::normalize(pDir - pCenter);

        dirAngle =atan2(nDir.y,nDir.x)+(3.1415f/2.f)+3.1415f;
        while( dirAngle >3.1415f)
        {
            dirAngle-=3.1415*2.0;
        }
        while( dirAngle < -3.1415f)
        {
            dirAngle+=3.1415*2.0;
        }
        makeMoveVector();
        resolveMoveVector();
    }else{
        motorSpeed.x =0;
        motorSpeed.y =0;
        motorSpeed.z =0;
    }


}
void PositionHandler::makeMoveVector()
{
  if(mousePos.x> 640 ||mousePos.y<0)
  {
      moveVector.x=0;
      moveVector.y=0;
  }else {
      moveVector.x = pCenter.x- mousePos.x;
      moveVector.y =  pCenter.y-mousePos.y ;
      moveVector.x *= 1;
      moveVector.y*= 1;
      float size =glm::length(moveVector);
float factor =1;
      if(size<40)
      {
          factor =(size)/40.f;

      }
     // cout<<factor<<endl;

      if (size>2.0)
      {

          moveVector/=size;
          moveVector*=2.0;

      }
      moveVector*=factor;
  }
}
void PositionHandler::resolveMoveVector()
{

    moveVectorR.x = moveVector.x*cos( dirAngle)-moveVector.y*sin(dirAngle);
    moveVectorR.y = moveVector.x*sin( dirAngle)+moveVector.y*cos(dirAngle);
    float dirAngleComp =dirAngle*5;
    if(dirAngleComp>1)dirAngleComp=1;
    float vecX =moveVector.x;
    float vecY =moveVector.y;
    motorSpeed.x =vecX -dirAngleComp;
    motorSpeed.y =vecX*cosM120-vecY *sinM120-dirAngleComp;
    motorSpeed.z =vecX*cos120-vecY *sin120-dirAngleComp;
cout<<dirAngle<<" " <<vecX<<" " <<vecY<<endl;
}
void PositionHandler::setMouse(int x,int y)
{
    mousePos.x =x;
    mousePos.y =y;

}
void PositionHandler::draw()
{
    glLineWidth(2);

    drawer.drawCircle( pCenter);

    drawer.drawLine( pCenter,pCenter+nDir*20.f);
    glLineWidth(4);
    drawer.drawLine(  mousePos-vec2(20,20), mousePos+vec2(20,20));
    drawer.drawLine(  mousePos-vec2(20,-20), mousePos+vec2(20,-20));
    //drawer.drawLine( pCenter,pCenter+moveVector*20.f);
    //drawer.drawLine( pCenter,pCenter+moveVectorR*30.f);
}