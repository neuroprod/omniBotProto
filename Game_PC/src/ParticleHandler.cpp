//
//  ParticleHandler.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 21/05/2017.
//
//

#include "ParticleHandler.hpp"
#include "glm/gtc/random.hpp"
#include "cinder/gl/gl.h"
#include "cinder/Perlin.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;
using namespace ci;
using namespace ci::app;


ParticleHandler::ParticleHandler()
{

}

ParticleHandler::~ParticleHandler()
{

}

void ParticleHandler::setup(float radius, glm::vec2 centerPos)
{
  

    gradientMap=gl::Texture::create( loadImage( loadAsset("gradient.png") ) );
    
    mGlsl = gl::GlslProg::create( loadAsset( "leave_shader.vert" ), loadAsset( "leave_shader.frag" ) );
  
    
    mGlslPlain = gl::GlslProg::create( loadAsset( "shadow_shader.vert" ), loadAsset( "shadow_shader.frag" ) );
    
    
    gl::VboMeshRef mesh= buildVBOMesh();
    
    float sqrRadius =radius*radius;
    std::vector<  glm::mat4> positions;
    std::vector<  glm::vec2> colors;
    ci::Perlin pnois=Perlin();
                            
                            
                            
       for(int i =0;i< numParticle;i++)
    {
        ParticleRef p= Particle::create();
        
        glm::vec2 randomPos;
        bool found =false;
        while(!found){
        
             randomPos = glm::vec2(glm::linearRand(-radius, radius),glm::linearRand(-radius, radius));
            if(glm::length2( randomPos)< sqrRadius  )
            {
                if(pnois.fBm(randomPos.x/200, randomPos.y/200)<-0.1)
                {
                
                    if(glm::linearRand(0.0, 1.0)>0.8)
                    {
                        found =true;
                    }
                    
                }else{
                
                    found =true;
                }
            }
        }
        
        p->friction =glm::linearRand(0.95f, 0.98f);
        
        float noise =( pnois.fBm(randomPos.x/500, randomPos.y/500)+0.3f)*3;
        colors.push_back(vec2(noise+glm::linearRand(-0.1f, 0.2f),glm::linearRand(0.3f, 0.8f)));
        
        p->scale=glm::linearRand(1.0f, 2.0f);
        
        
        p->rotation.x=glm::linearRand(0, 7);
        p->rotation.y=glm::linearRand(0, 7);
        p->rotation.z=glm::linearRand(0, 7);
       p->position.x  =randomPos.x+centerPos.x;
       p->position.y  =randomPos.y+centerPos.y;
        p->position.z =glm::linearRand(0.0f, 5.f);
      
        p->positionStart=  p->position;
        
        
       
        
        
      
        glm::mat4 t = glm::translate(vec3(p->position.x,p->position.y,p->position.z));
        glm::mat4 s =glm::scale(vec3(p->scale,p->scale,p->scale));
         glm::mat4 rx = glm::rotate(  p->rotation.x, vec3(1,0,0));
         glm::mat4 ry = glm::rotate(p->rotation.y, vec3(0,1,0));
         glm::mat4 rz = glm::rotate(p->rotation.z, vec3(0,0,1));
       
        p->matrix =t*rx*ry*rz *s;
       
       
        positions.push_back( p->matrix);
    
        particles.push_back(p);
    
    }
    
    mInstanceDataVbo = gl::Vbo::create( GL_ARRAY_BUFFER, positions.size() * sizeof(  glm::mat4), positions.data(), GL_DYNAMIC_DRAW );
    geom::BufferLayout instanceDataLayout;
    instanceDataLayout.append( geom::Attrib::CUSTOM_0, 16, sizeof( mat4 ), 0, 1 );
    mesh->appendVbo( instanceDataLayout, mInstanceDataVbo );
    
    mInstanceDataVboColor = gl::Vbo::create( GL_ARRAY_BUFFER, colors.size() * sizeof(  glm::vec2), colors.data(), GL_STATIC_DRAW );
    geom::BufferLayout instanceDataLayoutColor;
    instanceDataLayoutColor.append( geom::Attrib::CUSTOM_1, 2,0, 0, 1 );
    mesh->appendVbo( instanceDataLayoutColor, mInstanceDataVboColor );

    
    
    
    mBatch = gl::Batch::create( mesh, mGlsl, { { geom::Attrib::CUSTOM_0, "vInstancePosition" },{ geom::Attrib::CUSTOM_1, "vInstanceColor" }} );
    
    
    mBatchPlain = gl::Batch::create( mesh, mGlslPlain, { { geom::Attrib::CUSTOM_0, "vInstancePosition" } });
}



void ParticleHandler::update(double elapsed,PlayerRef player1,PlayerRef player2)
{
    glm::vec2 pos1=  player1->drawPosition2DFloor;
    glm::vec2 speed1= player1->moveSpeed2D;
    float speedSize1 = glm::length(speed1);
    
    glm::vec2 pos2=  player2->drawPosition2DFloor;
    glm::vec2 speed2= player2->moveSpeed2D;
    float speedSize2 = glm::length(speed2);
    
    
    float robotSize  =player1->robotSize+7;
    float robotsize2 = pow(robotSize,2.0f);
  
    glm::mat4 *positions = (glm::mat4 *)mInstanceDataVbo->mapReplace();
    
    for(auto p:particles)
    {
    
        glm::vec2 pPos =glm::vec2(p->position.x,p->position.y);
        
        ///player1
        float distance12 = glm::distance2(pos1, pPos);
        if(distance12<robotsize2  && p->position.z>-100)
        {
        
            p->hit  =true;
            glm::vec2 hitDir =pPos -pos1;
            float hitSize =robotSize -glm::length( hitDir);
            hitDir = glm::normalize(hitDir);
            glm::vec2 moveDir =hitDir*(hitSize+glm::linearRand(0.f, 0.f));
            p->position.x += moveDir.x;
            p->position.y += moveDir.y;
            
           
            
            float rand  =glm::linearRand(0.f, speedSize1/30.f);
            
            p->speed.x += moveDir.x*rand;
            p->speed.y += moveDir.y*rand;
            p->speed.z -=hitSize/10;
            p->position.z +=p->speed.z;
        
        }
        
        ///player2
        float distance22 = glm::distance2(pos2, pPos);
        if(distance22<robotsize2  && p->position.z>-100)
        {
            
            p->hit  =true;
            glm::vec2 hitDir =pPos -pos2;
            float hitSize =robotSize -glm::length( hitDir);
            hitDir = glm::normalize(hitDir);
            glm::vec2 moveDir =hitDir*(hitSize+glm::linearRand(0.f, 0.f));
            p->position.x += moveDir.x;
            p->position.y += moveDir.y;
            
            
            
            float rand  =glm::linearRand(0.f, speedSize2/30.f);
            
            p->speed.x += moveDir.x*rand;
            p->speed.y += moveDir.y*rand;
            p->speed.z -=hitSize/10;
            p->position.z +=p->speed.z;
            
        }

        
        
  
        
        
        p->rotation.x+=p->speed.x/20;
        p->rotation.y+=p->speed.z/20;
        
        p->speed*=p->friction;
        p->position.x+=p->speed.x;
        p->position.y+=p->speed.y;
        p->position.z+=p->speed.z;
        
        
        
        
        
        
        
        
        if(p->position.z!= p->positionStart.z)
        {
            p->speed.z+=0.1;
            if( p->speed.z>2) p->speed.z=2;
        }
        
        if(p->position.z> p->positionStart.z)
        {
            p->position.z =p->positionStart.z;
            p->speed.z =0;
        }
        
        
        p->updateMatrix();
        *positions++ =  p->matrix;
    }
    
    mInstanceDataVbo->unmap();
    
 
}

void ParticleHandler::draw(GameRenderer * renderer)
{
  
    
    gl::pushMatrices();
 

    mGlsl ->uniform( "uShadowMap", 0 );
    mGlsl ->uniform( "uGradientMap", 1 );
    mGlsl ->uniform( "uLightPos",  renderer->mvLightPos );
    mGlsl ->uniform( "uShadowMatrix",  renderer->shadowMatrix );
    gl::ScopedTextureBind texScope2( renderer->mShadowMapTex, (uint8_t) 0 );
    gl::ScopedTextureBind texScope3( gradientMap, (uint8_t) 1 );
    mBatch->drawInstanced( numParticle );
    
  gl::popMatrices();

    
    
}






void ParticleHandler::reset()
{
    for(auto p:particles)
    {
    
        p->position =p->positionStart;
        if(glm::linearRand(0.f, 1.f)>0.999)
        {
        
            p->position.z =glm::linearRand(0.f,-5000.f);
        
        }
        p->speed.x=100;
        p->updateMatrix();
           p->speed.x=0;
    }
    
}

gl::VboMeshRef ParticleHandler::buildVBOMesh()
{
   
    vector<vec3> posTemp;
    
    posTemp.push_back(vec3(0,-7,3));
    posTemp.push_back(vec3(-3.3,-3.5,1));
    posTemp.push_back(vec3(3.3,-3.5,1));
    posTemp.push_back(vec3(-4,0,0));
    posTemp.push_back(vec3(4,0,0));
    posTemp.push_back(vec3(-3.3,3.5,1));
    posTemp.push_back(vec3(3.3,3.5,1));
    posTemp.push_back(vec3(0,7,3));
    
    
    
    vector<unsigned short> indexTemp;
    
    indexTemp.push_back(0);
    indexTemp.push_back(2);
    indexTemp.push_back(1);
    
    indexTemp.push_back(1);
    indexTemp.push_back(2);
    indexTemp.push_back(3);
    
    indexTemp.push_back(3);
    indexTemp.push_back(2);
    indexTemp.push_back(4);
    
    indexTemp.push_back(3);
    indexTemp.push_back(4);
    indexTemp.push_back(5);
    
    indexTemp.push_back(5);
    indexTemp.push_back(4);
    indexTemp.push_back(6);
    
    indexTemp.push_back(5);
    indexTemp.push_back(6);
    indexTemp.push_back(7);
    
    
    
    
    
    auto vertices = vector<VertexData>();
    
    for(auto p:posTemp)
    {
        VertexData right;
        right.position =p;
        
        
        
        vertices.push_back(right);

    
    }
       auto layout = geom::BufferLayout();
    
    layout.append(geom::Attrib::POSITION, 3, sizeof(VertexData), offsetof(VertexData, position));
  
    
    // upload your data to the GPU
    auto buffer = gl::Vbo::create(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);
    
    // construct a VAO that describes the data in the buffer according to your layout.
   gl::VboMeshRef meshRef =gl::VboMesh::create(vertices.size(), GL_TRIANGLES, {{layout, buffer}},indexTemp.size());
    meshRef->bufferIndices(indexTemp.size()*sizeof(unsigned short), indexTemp.data());
    
    
    return  meshRef;
}



void ParticleHandler::drawShadow(GameRenderer * renderer)
{

   mBatchPlain->drawInstanced( numParticle );
    
  
}
