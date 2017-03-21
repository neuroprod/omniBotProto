//
// Created by Kris Temmerman on 21/03/17.
//

#include "BotTracker.h"
#include <iostream>
#include <stdio.h>

using namespace std;

void BotTracker::setup()
{
    cameraHandler.setup();
    window.setup(640,420);

    baseProgram.load();

    float sizeW =5.f/640.f;
    float sizeH =sizeW* 640.f/480.f;
    static const GLfloat quad_vertex_positions[] = {
            -sizeW, -sizeH,	1.0f, 1.0f,
            sizeW,-sizeH, 1.0f, 1.0f,
            -sizeW, sizeH, 1.0f, 1.0f,
            sizeW, sizeH, 1.0f, 1.0f
    };
    glGenBuffers(1, &GQuadVertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_positions), quad_vertex_positions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


}
void BotTracker::update(double ellapsed)
{

    cameraHandler.update();
    cout << ellapsed<<endl;
}
void BotTracker::draw()
{
    window.beginFrame();


    GLuint program = baseProgram.id;
    glUseProgram(program);
    float drawX = 0;
    float drawY= 0;
    glUniform2f(glGetUniformLocation(program,"offset"),drawX,drawY);
    glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);
    GLuint loc = glGetAttribLocation(program,"vertex");
    glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);
    glEnableVertexAttribArray(loc);
    glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    window.endFrame();
}