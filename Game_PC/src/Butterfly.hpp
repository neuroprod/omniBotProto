//
//  Butterfly.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 03/10/2017.
//
//

#ifndef Butterfly_hpp
#define Butterfly_hpp
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"


class Butterfly;
typedef std::shared_ptr<Butterfly> ButterflyRef;


class Butterfly
{
    
public:
    Butterfly(){}
    static ButterflyRef create();
 
    ci::vec3 position;
     ci::vec3 positionDraw;
  ci::vec3 speed;
    ci::vec2 noiseOffset;
    
    float random;
};

#endif /* Butterfly_hpp */
