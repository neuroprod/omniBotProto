//
// Created by Kris Temmerman on 21/03/17.
//

#ifndef BOTTRACKER_BOTTRACKER_H
#define BOTTRACKER_BOTTRACKER_H
#include "CameraHandler.h"
#include "GLWindow.h"
#include "GLProgram.h"
class BotTracker {

public:
    BotTracker(){}
    void setup();
    void update(double elapsed);
    void draw();
    CameraHandler cameraHandler;
    GLWindow window;
    GLProgram  baseProgram;

    GLuint GQuadVertexBuffer;
};


#endif //BOTTRACKER_BOTTRACKER_H
