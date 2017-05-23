//
//  Particle.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 21/05/2017.
//
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "cinder/app/App.h"

class Particle;
typedef std::shared_ptr<Particle> ParticleRef;

class Particle
{


public:
    Particle();
    
    static ParticleRef create();
    glm::vec4 positionStart;
    glm::vec4 position;
    glm::vec3 speed;
    float friction;
    bool hit;

};


#endif /* Particle_hpp */
