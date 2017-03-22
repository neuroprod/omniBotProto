//
// Created by pi on 22/03/17.
//
#include <glm/glm.hpp>
#include "PositionHandler.h"
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
    }


}

void PositionHandler::draw()
{
    glLineWidth(2);

    drawer.drawCircle( pCenter);

    drawer.drawLine( pCenter,pCenter+nDir*20.f);
}