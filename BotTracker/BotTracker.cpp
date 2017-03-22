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
    cameraHandler.drawTexture =true;

    positionHandler.setup();

    glm::vec3 a =glm::vec3(10.f,10.f,10.f);

}
void BotTracker::update(double ellapsed)
{

   cameraHandler.update();
    positionHandler.setPoints(cameraHandler.centers);
    //cout<< 1000/ellapsed<<endl;
}
void BotTracker::draw()
{

    window.beginFrame();
    cameraHandler.draw();
    positionHandler.draw();
    window.endFrame();
}