//
//  Calibrator.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 24/05/2017.
//
//

#ifndef Calibrator_hpp
#define Calibrator_hpp

#include <stdio.h>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Surface.h"
#include "delaunay.h"

class Calibrator
{

public:
    
    Calibrator();
    void setup(std::string type ="camera");
    
    void addVector(ci::vec2 v);
    
    void draw();
    void loadCalibration();
    void saveCalibration();
     void reset();
    void makeDelaunay();
    std::string type ;
    bool loaded =false;
    std::vector<TriVec> vertices;
    ci::gl::FboRef			mFbo;
    
    ci::vec2 getOffsetForPoint(ci::ivec2 point);

    ci::SurfaceRef surface;
    
    float offX =0;
    float offY =0;
    
};

#endif /* Calibrator_hpp */
