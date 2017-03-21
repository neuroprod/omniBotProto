//
// Created by Kris Temmerman on 21/03/17.
//

#include "GLTexture.h"
#include <iostream>
#include <stdio.h>

using namespace std;
bool GLTexture::create(int width, int height, const void* data)
{
    Width = width;
    Height = height;
    glGenTextures(1, &Id);

    glBindTexture(GL_TEXTURE_2D, Id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width,Height , 0,  GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void  GLTexture::setPixels(const void* data)
{
    glBindTexture(GL_TEXTURE_2D, Id);
    cout << " ERROR1"<<glGetError()<<endl;
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, Height,  GL_RGB, GL_UNSIGNED_BYTE, data);
    cout << " ERROR2"<<glGetError()<<endl;
    glBindTexture(GL_TEXTURE_2D, 0);

}

void GLTexture::setMat( cv::Mat &mat)
{
    if(Id==0)
    {
        create(mat.cols,mat.rows, mat.ptr());
        return;
    }
    glBindTexture(GL_TEXTURE_2D, Id);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,  mat.cols,mat.rows,  GL_RGB, GL_UNSIGNED_BYTE, mat.ptr());

    glBindTexture(GL_TEXTURE_2D, 0);

}
