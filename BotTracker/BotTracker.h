//
// Created by Kris Temmerman on 21/03/17.
//

#ifndef BOTTRACKER_BOTTRACKER_H
#define BOTTRACKER_BOTTRACKER_H
#include "CameraHandler.h"
#include "PositionHandler.h"
#include "GLWindow.h"
#include "GLProgram.h"
#include "RadioHandler.h"
#include "KeyHandler.h"
class BotTracker {

public:
    BotTracker(){}
    void setup();
    void update(double elapsed);
    void draw();

    PositionHandler positionHandler;
    CameraHandler cameraHandler;
    GLWindow window;
    RadioHandler radioHandler;
    KeyHandler keyHandler;
};


#endif //BOTTRACKER_BOTTRACKER_H
