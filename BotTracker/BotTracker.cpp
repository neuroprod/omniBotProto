//
// Created by Kris Temmerman on 21/03/17.
//

#include "BotTracker.h"
#include <iostream>
#include <stdio.h>

using namespace std;

void BotTracker::setup()
{
    cameraHandler.setup();

    window.setup(640,420);
    cameraHandler.setupDraw();
    cameraHandler.drawTexture =true;
    //cameraHandler.drawTexture =true;



}
void BotTracker::update(double ellapsed)
{

   cameraHandler.update();
cout<< 1000/ellapsed<<endl;
}
void BotTracker::draw()
{

    window.beginFrame();
    cameraHandler.draw();

    window.endFrame();
}