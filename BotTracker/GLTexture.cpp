//
// Created by Kris Temmerman on 21/03/17.
//

#include "GLTexture.h"

bool GLTexture::create(int width, int height, const void* data)
{
    Width = width;
    Height = height;
    glGenTextures(1, &Id);

    glBindTexture(GL_TEXTURE_2D, Id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, Width, Height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void  GLTexture::setPixels(const void* data)
{
    glBindTexture(GL_TEXTURE_2D, Id);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, Height, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);

}
