//
//  GrassPatch.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 30/06/2017.
//
//

#include "GrassPatch.hpp"
#include "glm/gtc/random.hpp"
#include "cinder/Perlin.h"

using namespace ci;
using namespace ci::app;
using namespace std;


GrassPatchRef GrassPatch::create()
{
    return  make_shared<GrassPatch>();
}
void GrassPatch::addGrass(float x,float y,float xL,float yL,float size2,ci::Surface &gradient)
{
    Perlin perlin;
    perlin.setSeed(20);

    vec2 rot = vec2(glm::linearRand(-1.f,1.f),glm::linearRand(-2.f,1.f));
    
    rot =glm::normalize(rot);
    
    float noise = perlin.fBm(vec2(xL/500.f,yL/500.f) )*2+0.5+glm::linearRand(-0.5f,0.5f);
    if(noise<0)noise =0;
    if(noise>1)noise =1;
    
    ci::ColorA grassColor =  gradient.getPixel(vec2(noise*255,0));
    
    
    color.push_back(vec3(grassColor.r,grassColor.g, grassColor.b));
    texCoord0.push_back(vec3(rot.x,rot.y, glm::linearRand(10.f,20.f)));
    vertex.push_back(vec2(x-size2,y-size2));
    
    
    float f =1;
    flat.push_back(f);
}

void GrassPatch::updatePlayer(ci::vec2 playerPos)
{

    for(int i=0;i< flat.size();i++)
    {
        if(glm::distance2(playerPos,vertex[i] )<2500)
        {
        
            flat[i]=0;
        }
    
    }

}
void GrassPatch::update()
{

    auto mappedPosAttrib = mVboMesh->mapAttrib1f( geom::Attrib::TEX_COORD_1, false );
    for( int i = 0; i < mVboMesh->getNumVertices(); i++ ) {
        float &pos = *mappedPosAttrib;
        
        if(flat[i]<0.3)
        {
            flat[i]+=0.01;
           
        }
        
        pos = flat[i];
        ++mappedPosAttrib;
    }
    mappedPosAttrib.unmap();

}
void GrassPatch::setup(){
    
    
    
    
    auto layoutV = geom::BufferLayout();
    layoutV.append(geom::Attrib::POSITION, 2, sizeof(vec2),0);
    auto bufferV = gl::Vbo::create(GL_ARRAY_BUFFER,  vertex, GL_STATIC_DRAW);
    
    
    auto layoutC = geom::BufferLayout();
    layoutC.append(geom::Attrib::COLOR, 3, sizeof(vec3),0);
    auto bufferC = gl::Vbo::create(GL_ARRAY_BUFFER,  color, GL_STATIC_DRAW);

    
    auto layoutU = geom::BufferLayout();
    layoutU.append(geom::Attrib::TEX_COORD_0, 3, sizeof(vec3),0);
    auto bufferU = gl::Vbo::create(GL_ARRAY_BUFFER,   texCoord0, GL_STATIC_DRAW);

    auto layoutF = geom::BufferLayout();
    layoutF.append(geom::Attrib::TEX_COORD_1, 1, sizeof(float),0);
    auto bufferF = gl::Vbo::create(GL_ARRAY_BUFFER,   flat, GL_DYNAMIC_DRAW);

   
    // construct a VAO that describes the data in the buffer according to your layout.
    mVboMesh  =gl::VboMesh::create(vertex.size(), GL_POINTS, {{layoutV, bufferV},{layoutC, bufferC},{layoutU, bufferU},{layoutF, bufferF}});
  

    
    
  
}

void GrassPatch::draw()
{

    gl::draw(mVboMesh);
}
