//
//  Calibrator.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 24/05/2017.
//
//

#include "Calibrator.hpp"
#include "glm/gtc/random.hpp"
#include <fstream>
#include "cinder/Utilities.h"

using namespace std;
using namespace ci;
using namespace ci::app;
Calibrator::Calibrator(){}
void Calibrator::setup(string _type)
{
    type = _type;
    mFbo = gl::Fbo::create( 1280, 720 );
    gl::ScopedFramebuffer fbo( mFbo );
    gl::clear( Color::gray(0.5));
    
    surface= ci::Surface::create( mFbo->getColorTexture()->createSource());

}
void Calibrator::addVector(vec2 v)
{
    TriVec vec;
    vec.x =v.x;
    vec.y =v.y;
    vec.offX =offX;
    vec.offY =offY;
    vertices.push_back(vec);
    makeDelaunay();
}

void Calibrator::draw()
{
    gl::color(Color::gray(0.5));
    gl::draw(mFbo->getColorTexture());
}
vec2 Calibrator::getOffsetForPoint(ivec2 point)
{

   
    ColorA color= surface->getPixel(point);
   
    vec2 offset;
    offset.x = color.r;
    offset.y = color.g;
    
    
    offset-=0.5;
    offset*=25.6;

    
    return offset;
}

void Calibrator::makeDelaunay()
{
    Delaunay delaunay;
    delaunay.triangulate(vertices);
    vector<Triangle>triangles = delaunay.getTriangles();
    
    
    gl::ScopedFramebuffer fbo( mFbo );
    gl::clear( Color::gray(0.0));
    gl::color(1,1,1);
     gl::begin(GL_TRIANGLES);
    for(auto t:triangles)
    {
        
        gl::color(t.p1.offX/25.6 +0.5,t.p1.offY/25.6 +0.5,0 );
        gl::vertex(t.p1.get());
        gl::color(t.p2.offX/25.6 +0.5,t.p2.offY/25.6 +0.5,0 );
        gl::vertex(t.p2.get());
        gl::color(t.p3.offX/25.6 +0.5,t.p3.offY/25.6 +0.5,0 );
        gl::vertex(t.p3.get());
    
    }
    gl::end();
    surface= ci::Surface::create( mFbo->getColorTexture()->createSource());
    
}
void Calibrator::reset()
{

    offX =0;
    offY =0;
    vertices.clear();
    {
        TriVec vec;
        vec.x =0;
        vec.y =0;
        vec.offX =0;
        vec.offY =0;
        vertices.push_back(vec);
    }
    
    {
        TriVec vec;
        vec.x =1280;
        vec.y =0;
        vec.offX =0;
        vec.offY =0;
        vertices.push_back(vec);
    }
    
    {
        TriVec vec;
        vec.x =0;
        vec.y =720;
        vec.offX =0;
        vec.offY =0;
        vertices.push_back(vec);
    }
    
    {
        TriVec vec;
        vec.x =1280;
        vec.y =720;
        vec.offX =0;
        vec.offY =0;
        vertices.push_back(vec);
    }
    
    makeDelaunay();
}
void Calibrator::loadCalibration()
{
 
    std::ifstream infile(type+ "_cal.txt");
    if(infile){
        std::string line;
        while (std::getline(infile, line))
        {
       
            vector<string> substrings = split( line, " ," );
            TriVec vec;
            vec.x =stof(substrings[0]);
            vec.y =stof(substrings[1]);
            vec.offX =stof(substrings[2]);
            vec.offY =stof(substrings[3]);
            vertices.push_back(vec);
            
            
        }
        makeDelaunay();
    }else
    {
        console()<<"NO CALIBRATION FILE"<<endl;
    
    }
   //
    loaded =true;
}

void Calibrator::saveCalibration()
{

    std::string myPath =type+ "_cal.txt";
    
    // Get an ofstream which is what you'll use to write to your file.
    std::ofstream oStream( myPath );
    for(auto v: vertices)
    {
        oStream << to_string(v.x) << ","<< to_string(v.y) << ","<< to_string(v.offX) << ","<< to_string(v.offY) << "," <<endl;
    
    }
    
    
    oStream.close();


}
