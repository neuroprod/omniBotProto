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
    void draw();
    PositionDrawer drawer;

    glm::vec2 pCenter;
    glm::vec2 pDir;
    glm::vec2 nDir;
};


#endif //BOTTRACKER_POSITIONHANDLER_H
