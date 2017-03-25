//
// Created by Kris Temmerman on 21/03/17.
//
#include <glm/vec3.hpp>
#include "BotTracker.h"

#include <iostream>
#include <stdio.h>

using namespace std;

void BotTracker::setup()
{
    bcm_host_init();

    window.setup(640,480);

    cameraHandler.setup();
    cameraHandler.setupDraw();
    //cameraHandler.drawTexture =true;

    positionHandler.setup();
   radioHandler.setup();
    keyHandler.GScreenHeight =window.GScreenHeightT;
    keyHandler.GScreenWidth =window.GScreenWidthT;
}
void BotTracker::update(double ellapsed)
{
    cameraHandler.update();
    keyHandler.update();
    positionHandler.setPoints(cameraHandler.centers);
    positionHandler.setMouse(keyHandler.xapp,480-keyHandler.yapp);
   radioHandler.send(0,positionHandler.motorSpeed.x,positionHandler.motorSpeed.y,positionHandler.motorSpeed.z);
    //cout<< 1000/ellapsed<<endl;
}
void BotTracker::draw()
{

    window.beginFrame();
    cameraHandler.draw();
    positionHandler.draw();
    window.endFrame();
}