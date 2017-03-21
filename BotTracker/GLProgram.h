//
// Created by Kris Temmerman on 21/03/17.
//

#ifndef BOTTRACKER_GLPROGRAM_H
#define BOTTRACKER_GLPROGRAM_H

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"


class GLProgram {
    public:
    GLProgram(){};
    void load();
    void bind();
    GLuint loadShader(const char* filename, GLenum type);
    void  printShaderInfoLog(GLint shader);
    GLchar* Src;

    GLuint id;
};


#endif //BOTTRACKER_GLPROGRAM_H
