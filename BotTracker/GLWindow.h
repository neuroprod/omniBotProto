//
// Created by Kris Temmerman on 21/03/17.
//

#ifndef BOTTRACKER_GLWINDOW_H
#define BOTTRACKER_GLWINDOW_H

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"


class GLWindow {
public:
    GLWindow (){}
    void setup(int width, int height);

    void beginFrame();
    void endFrame();



    uint32_t GScreenWidth;
    uint32_t GScreenHeight;
    EGLDisplay GDisplay;
    EGLSurface GSurface;
    EGLContext GContext;
};


#endif //BOTTRACKER_GLWINDOW_H
