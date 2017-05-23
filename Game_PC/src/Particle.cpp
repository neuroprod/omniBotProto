//
//  Particle.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 21/05/2017.
//
//

#include "Particle.hpp"


using namespace std;
using namespace ci;

Particle::Particle(){}

ParticleRef  Particle::create()
{
    return make_shared<Particle>();
}

void Particle::updateMatrix()
{
    if(length2(speed)<0.01)return;
    
    glm::mat4 t = glm::translate(vec3(position.x,position.y,position.z));
    glm::mat4 s =glm::scale(vec3(scale,scale,scale));
    glm::mat4 rx = glm::rotate( rotation.x, vec3(1,0,0));
    glm::mat4 ry = glm::rotate(rotation.y, vec3(0,1,0));
    glm::mat4 rz = glm::rotate(rotation.z, vec3(0,0,1));
    matrix=  t*rx*ry*rz *s;

}
