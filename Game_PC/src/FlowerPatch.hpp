//
//  FlowerPatch.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 03/10/2017.
//
//

#ifndef FlowerPatch_hpp
#define FlowerPatch_hpp


#include "cinder/app/App.h"
#include "cinder/gl/gl.h"


class FlowerPatch;
typedef std::shared_ptr<FlowerPatch> FlowerPatchRef;


class FlowerPatch
{
    
public:
    FlowerPatch(){}
    static FlowerPatchRef create();
    
    void setup();
    void updatePlayer(ci::vec2 playerPos);
    void update();
    void draw();
    void addFlower(float x,float y,float xL,float yL,float size2, ci::Surface &gradient);
    
    
    std::vector<glm::vec3> color;
    std::vector<glm::vec3> texCoord0;
    std::vector<glm::vec2> vertex;
    std::vector<float> flat;
    ci::gl::VboMeshRef	mVboMesh;
    
};


#endif /* FlowerPatch_hpp */
