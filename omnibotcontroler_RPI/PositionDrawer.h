//
// Created by pi on 22/03/17.
//

#ifndef BOTTRACKER_POSITIONDRAWER_H
#define BOTTRACKER_POSITIONDRAWER_H
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <stdio.h>
#include "GLProgram.h"
class PositionDrawer {
public:
    PositionDrawer(){};
    void setup();
    void drawCircle(glm::vec2 p);
    void drawLine(glm::vec2 p1, glm::vec2 p2);
    glm::vec2 makeGL(glm::vec2 p);
    GLProgram colorProgram;
    float sWidth;
    float sHeight;

    GLuint circleVertexBuffer;
   int circleVertexSize;
};


#endif //BOTTRACKER_POSITIONDRAWER_H
