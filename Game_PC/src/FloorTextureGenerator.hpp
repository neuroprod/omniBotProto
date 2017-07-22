//
//  FloorTextureGenerator.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 03/07/2017.
//
//

#ifndef FloorTextureGenerator_hpp
#define FloorTextureGenerator_hpp
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Surface.h"
#include "delaunay.h"


class FloorTextureGenerator
{
public:
    FloorTextureGenerator(){}
    void setup();
    void generate();
    std::vector<TriVec> vertices;
    ci::gl::FboRef			mFbo;
    

    
    ci::SurfaceRef surface;
    
    float offX =0;
    float offY =0;
    std::vector<Triangle>triangles;
    bool generated =false;
    
    float size;
    float noiseSize;
    ci::SurfaceRef noise;
    ci::SurfaceRef gradient;
};

#endif /* FloorTextureGenerator_hpp */
