//
// Created by Kris Temmerman on 21/03/17.
//

#ifndef BOTTRACKER_GLTEXTURE_H
#define BOTTRACKER_GLTEXTURE_H
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include <opencv2/opencv.hpp>

class GLTexture {
    int Width;
    int Height;


public:

    GLTexture() : Width(0), Height(0), Id(0)  {}
    ~GLTexture() {}

    bool create(int width, int height, const void* data = NULL);
    void setPixels(const void* data);
    void setMat( cv::Mat &mat);
    GLuint Id ;
};


#endif //BOTTRACKER_GLTEXTURE_H
