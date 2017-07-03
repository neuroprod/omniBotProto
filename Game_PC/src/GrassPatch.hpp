//
//  GrassPatch.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 30/06/2017.
//
//

#ifndef GrassPatch_hpp
#define GrassPatch_hpp

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"


class GrassPatch;
typedef std::shared_ptr<GrassPatch> GrassPatchRef;


class GrassPatch
{

public:
    GrassPatch(){}
    static GrassPatchRef create();
    
    void setup();
    void updatePlayer(ci::vec2 playerPos);
    void update();
    void draw();
    void addGrass(float x,float y,float xL,float yL,float size2, ci::Surface &gradient);
    
    
    std::vector<glm::vec3> color;
    std::vector<glm::vec3> texCoord0;
    std::vector<glm::vec2> vertex;
    std::vector<float> flat;
    ci::gl::VboMeshRef	mVboMesh;

};
#endif /* GrassPatch_hpp */
