//
//  FloorTextureGenerator.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 03/07/2017.
//
//

#include "FloorTextureGenerator.hpp"
#include "glm/gtc/random.hpp"
#include "cinder/Perlin.h"
using namespace std;
using namespace ci;
using namespace ci::app;



void FloorTextureGenerator::setup()
{
     size = 2512;
    mFbo = gl::Fbo::create( size, size );
    gl::ScopedFramebuffer fbo( mFbo );
    gl::clear( Color::gray(0.5));
    
    gradient =  Surface::create(loadImage( loadAsset("floor/floorgradient.png") ));
    noise =  Surface::create(loadImage( loadAsset("noise.jpg") ));
    noiseSize = noise->getWidth();

}
void FloorTextureGenerator::generate(){
    
    if(generated ) return;
    generated =true;
    gl::ScopedFramebuffer fbo( mFbo );
    gl::clear( Color::gray(0));
    gl::pushMatrices();
 gl::setMatricesWindow(  vec2(size,size));
    glViewport(0,0, size , size );
   
   
    
    
    gl::disableDepthWrite();
    
    // render the color cube
    gl::ScopedGlslProg shaderScp( gl::getStockShader( gl::ShaderDef().color() ) );
    gl::color(1, 0, 0);
    gl::drawSolidTriangle(vec2(0,0), vec2(0,size), vec2(size,size));
   std::vector<TriVec> vertices;
    
    Perlin p;
    
    srand(0);
    for(int i=0;i< 500;i++)
    {
        
        TriVec vec;
        vec.x =glm::linearRand(0.f,size);
        vec.y =glm::linearRand(0.f,size);
        vec.offX =0;
        vec.offY =0;
        vertices.push_back(vec);
        
        
        TriVec vecl;
        vecl.x =vec.x-size ;
        vecl.y =vec.y;
        vecl.offX =0;
        vecl.offY =0;
        vertices.push_back(vecl);
        
        TriVec vecr;
        vecr.x =vec.x+size ;
        vecr.y =vec.y;
        vecr.offX =0;
        vecr.offY =0;
        vertices.push_back(vecr);
        
        
        TriVec vect;
        vect.x =vec.x ;
        vect.y =vec.y-size;
        vect.offX =0;
        vect.offY =0;
        vertices.push_back(vect);
        
        TriVec vecb;
        vecb.x =vec.x ;
        vecb.y =vec.y+size;
        vecb.offX =0;
        vecb.offY =0;
        vertices.push_back(vecb);

        
        TriVec veclt;
        veclt.x =vec.x-size ;
        veclt.y =vec.y-size;
        veclt.offX =0;
        veclt.offY =0;
        vertices.push_back(veclt);
        
        TriVec vecrt;
        vecrt.x =vec.x+size ;
        vecrt.y =vec.y+size;
        vecrt.offX =0;
        vecrt.offY =0;
        vertices.push_back(vecrt);
        
        
        TriVec vectl;
        vectl.x =vec.x +size;
        vectl.y =vec.y-size;
        vectl.offX =0;
        vectl.offY =0;
        vertices.push_back(vectl);
        
        TriVec vecbr;
        vecbr.x =vec.x -size;
        vecbr.y =vec.y+size;
        vecbr.offX =0;
        vecbr.offY =0;
        vertices.push_back(vecbr);
        
        
        
        
    }
    
    
    
    
    Delaunay delaunay;
    delaunay.triangulate(vertices);
    triangles = delaunay.getTriangles();
    
    
  
    
  
  
    
    for(auto t:triangles)
    {
        vec2 center  =(t.p1.get()+ t.p2.get()+ t.p3.get())/3.0f;
        
    
       center = glm::mod(center,vec2(size,size));
        
        vec2 pos =center/size*noiseSize;
        ColorA c= noise->getPixel(ivec2(pos.x,pos.y));

        ColorA b =gradient->getPixel(ivec2(c.r*255,0));

        gl::color(b.r*0.8,b.g*0.8,b.b*0.8);
        gl::drawSolidTriangle(t.p1.get(), t.p2.get(), t.p3.get() );
        
        
    }
  
    gl::popMatrices();


}
