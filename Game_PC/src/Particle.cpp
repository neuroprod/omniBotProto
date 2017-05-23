//
//  Particle.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 21/05/2017.
//
//

#include "Particle.hpp"


using namespace std;
Particle::Particle(){}

ParticleRef  Particle::create()
{
    return make_shared<Particle>();
}
