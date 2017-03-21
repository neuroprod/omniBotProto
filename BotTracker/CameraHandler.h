//
// Created by Kris Temmerman on 21/03/17.
//

#ifndef BOTTRACKER_CAMERAHANDLER_H
#define BOTTRACKER_CAMERAHANDLER_H

#include "camera/camera.h"
#include <opencv2/opencv.hpp>
#include "GLTexture.h"
class CameraHandler {

public:
    CameraHandler(){}
    void setup();
    void update();


    CCamera* cam ;
    int camWidth ;
    int camHeight ;
    GLTexture texture;
};


#endif //BOTTRACKER_CAMERAHANDLER_H
