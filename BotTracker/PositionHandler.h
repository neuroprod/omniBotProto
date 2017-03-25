//
// Created by pi on 22/03/17.
//

#ifndef BOTTRACKER_POSITIONHANDLER_H
#define BOTTRACKER_POSITIONHANDLER_H
#include <glm/vec2.hpp>
#include "PositionDrawer.h"
#include <iostream>
#include <stdio.h>
#include <vector>

class PositionHandler {
public:
    PositionHandler(){};
    void setup();
    void setPoints(std::vector<glm::vec2>);
    void setMouse(int x,int y);
    void draw();
    void makeMoveVector();
    void resolveMoveVector();
    PositionDrawer drawer;

    glm::vec2 pCenter;
    glm::vec2 pDir;
    glm::vec2 nDir;
    glm::vec3 motorSpeed;
    glm::vec2 moveVector;
    glm::vec2 mousePos;
};


#endif //BOTTRACKER_POSITIONHANDLER_H
