//
// Created by Kris Temmerman on 21/03/17.
//

#ifndef BOTTRACKER_GLTEXTURE_H
#define BOTTRACKER_GLTEXTURE_H
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"


class GLTexture {
    int Width;
    int Height;
    GLuint Id;

public:

    GLTexture() : Width(0), Height(0) {}
    ~GLTexture() {}

    bool create(int width, int height, const void* data = NULL);
    void setPixels(const void* data);
    GLuint GetId() { return Id; }
};


#endif //BOTTRACKER_GLTEXTURE_H
