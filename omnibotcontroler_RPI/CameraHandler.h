//
// Created by Kris Temmerman on 21/03/17.
//

#ifndef BOTTRACKER_CAMERAHANDLER_H
#define BOTTRACKER_CAMERAHANDLER_H

#include <glm/vec2.hpp>
#include "camera/camera.h"
#include <opencv2/opencv.hpp>
#include "GLTexture.h"
#include "GLProgram.h"
#include <iostream>
#include <stdio.h>

class CameraHandler {

public:
    CameraHandler(){}
    void setup();
    void setupDraw();
    void update();
    void draw();

    CCamera* cam ;
    int camWidth ;
    int camHeight ;

    bool drawTexture;

    GLTexture texture;
    GLProgram  baseProgram;
    GLuint GQuadVertexBuffer;
    std::vector<glm::vec2> centers;
};


#endif //BOTTRACKER_CAMERAHANDLER_H
