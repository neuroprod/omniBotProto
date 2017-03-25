//
// Created by pi on 22/03/17.
//

#include "PositionDrawer.h"
#include <vector>
using namespace std;
using namespace glm;
void PositionDrawer::setup()
{
    sWidth=640;
    sHeight=480;
    colorProgram.load("colorShader");
    vector<vec2> positions;
    int steps=20;
    float size =60;

    for(int i=0;i< steps;i++)
    {
        vec2 a0 = vec2(0,0);

        float angle1 =((float)i/ steps)*3.1415*2;
        float angle2 =(((float)i+1.f)/ steps) *3.1415*2;

        vec2 a1 ;
        a1.x = cos(angle1)/sWidth *size;
        a1.y= sin(angle1)/sHeight *size;

        vec2 a2 ;
        a2.x = cos(angle2)/sWidth *size;
        a2.y= sin(angle2)/sHeight *size;


        positions.push_back(a0);
        positions.push_back(a1);
        positions.push_back(a2);
    }
    circleVertexSize =positions.size();
   GLfloat vertex_positions[ positions.size()*2] ;
    for(int i=0;i<positions.size();i++)
    {
        int stride = i*2;
        vertex_positions[stride] = positions[i].x;
        vertex_positions[stride+1] = positions[i].y;
    }

    glGenBuffers(1, &circleVertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, circleVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof( vertex_positions),vertex_positions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


}
void PositionDrawer::drawCircle(glm::vec2 p)
{

    GLuint program = colorProgram.id;
    glUseProgram(program);
    glm::vec2 pd =makeGL(p);
    glUniform2f(glGetUniformLocation(program, "offset"), pd.x,pd.y);
    glUniform3f(glGetUniformLocation(program, "color"), 0.0666, 0.5411,0.698);

    glBindBuffer(GL_ARRAY_BUFFER, circleVertexBuffer);



    GLuint loc = glGetAttribLocation(program, "vertex");
    glVertexAttribPointer(loc, 2, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(loc);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, circleVertexSize);

    glFinish();
    glFlush();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);


}
void PositionDrawer::drawLine(glm::vec2 p1, glm::vec2 p2)
{
    GLuint program = colorProgram.id;
    glUseProgram(program);

    glUniform2f(glGetUniformLocation(program, "offset"), 0, 0);
    glUniform3f(glGetUniformLocation(program, "color"), 1.0, 1.0,1.0);
    glm::vec2 pd1 =makeGL(p1);
    glm::vec2 pd2 =makeGL(p2);
    GLuint loc = glGetAttribLocation(program, "vertex");

    float a[4]={pd1.x ,pd1.y ,pd2.x ,pd2.y };
    glVertexAttribPointer(loc, 2, GL_FLOAT, 0, sizeof(float)*2, &a);
    glEnableVertexAttribArray(loc);

    glDrawArrays(GL_LINES, 0, 2);

    glFinish();
    glFlush();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);


}
glm::vec2 PositionDrawer::makeGL(glm::vec2 p)
{
    p.x/=sWidth/2;
    p.x-=1;

    p.y/= sHeight/2;
    p.y-=1;
    return p;
}
