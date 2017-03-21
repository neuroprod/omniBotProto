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


    static const GLfloat quad_vertex_positions[] = {
            0.0f, 0.0f,	1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f
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

GLuint program= baseProgram.id;
    glUseProgram(program);


    glUniform2f(glGetUniformLocation(program,"offset"),-1,-1);
    glUniform2f(glGetUniformLocation(program,"scale"),2,2);
    glUniform1i(glGetUniformLocation(program,"tex"), 0);


    glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);


    glBindTexture(GL_TEXTURE_2D,cameraHandler.texture.Id);


    GLuint loc = glGetAttribLocation(program,"vertex");


    glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);


    glEnableVertexAttribArray(loc);


    glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );


    glFinish();


    glFlush();


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);



    window.endFrame();
}