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
  
    float viewAngle =10;
    float distance = screenHeight / 2 / sinf(glm::radians(viewAngle / 2))*  sinf(glm::radians(90 - viewAngle / 2));
    
    camera.setEyePoint(vec3(screenWidth / 2, screenHeight / 2, -distance));
    camera.setPerspective(viewAngle, screenWidth / screenHeight, 1, distance + 500);
    camera.lookAt(vec3(screenWidth / 2, screenHeight / 2, 0));
    camera.setWorldUp(vec3(0, 0, 1));
    
   
    
    setupShadow();
    

       updateCameraPosition();
 
    gradientMap=gl::Texture::create( loadImage( loadAsset("gradient.png") ) );
    
    mGlsl = gl::GlslProg::create( loadAsset( "shader.vert" ), loadAsset( "shader.frag" ) );
  
    
     mGlslPlain = gl::GlslProg::create( loadAsset( "shaderPlain.vert" ), loadAsset( "shaderPlain.frag" ) );
    
    
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
void ParticleHandler::updateCameraPosition()
{

    cameraPosition.x=0;
    cameraPosition.y=-screenHeight-offyCam ;
    cameraPosition.z=offzCam;

    cameraProj.setEyePoint(vec3(screenWidth / 2, screenHeight+offyCam /2,offzCam/2));
    
    cameraProj.lookAt(vec3(screenWidth / 2,screenHeight+offyCam /2, 0));
    cameraProj.setCameraPos(cameraPosition);
   

}



void ParticleHandler::update(double elapsed,PlayerRef player)
{
    glm::vec2 pos=  player->drawPosition2DFloor;
    glm::vec2 speed= player->moveSpeed2D;
    float speedSize = glm::length(speed);
    float robotSize  =player->robotSize+7;
    float robotsize2 = pow(robotSize,2.0f);
  
    glm::mat4 *positions = (glm::mat4 *)mInstanceDataVbo->mapReplace();
    
    for(auto p:particles)
    {
    
        glm::vec2 pPos =glm::vec2(p->position.x,p->position.y);
        float distance2 = glm::distance2(pos, pPos);
        if(distance2<robotsize2  && p->position.z>-100)
        {
        
            p->hit  =true;
            glm::vec2 hitDir =pPos -pos;
            float hitSize =robotSize -glm::length( hitDir);
            hitDir = glm::normalize(hitDir);
            glm::vec2 moveDir =hitDir*(hitSize+glm::linearRand(0.f, 0.f));
            p->position.x += moveDir.x;
            p->position.y += moveDir.y;
            
           
            
            float rand  =glm::linearRand(0.f, speedSize/30.f);
            
            p->speed.x += moveDir.x*rand;
            p->speed.y += moveDir.y*rand;
            p->speed.z -=hitSize/20;
            p->position.z +=p->speed.z;
        
        }else
        {
        
            p->hit =false;
           
        }
    
       // console()<<p->speed.z<<" "<<p->position.z<<endl;
        
        
        p->rotation.x+=p->speed.x/20;
        p->rotation.y+=p->speed.z/20;
        
        p->speed*=p->friction;
        p->position.x+=p->speed.x;
        p->position.y+=p->speed.y;
         p->position.z+=p->speed.z;
        
        
        
        
        
        
        
        
        if(p->position.z!= p->positionStart.z){
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
    
    renderDepthFbo();
}

void ParticleHandler::draw()
{
    gl::enableDepthRead();
    gl::enableDepthWrite();
     gl::setMatrices( cameraProj);
    
    gl::ScopedTextureBind texScope( mShadowMapTex, (uint8_t) 0 );
    
    vec3 mvLightPos	= vec3( gl::getModelView() * vec4( mLightPos, 1.0f ) ) ;
    mat4 shadowMatrix = mLightCam.getProjectionMatrix() * mLightCam.getViewMatrix();
    
    mGlslFloor ->uniform( "uShadowMap", 0 );
    mGlslFloor ->uniform( "uLightPos", mvLightPos );
    mGlslFloor ->uniform( "uShadowMatrix", shadowMatrix );

    
    
    gl::pushMatrices();
    gl::translate(screenWidth/2, screenHeight/2, +10);
    gl::rotate(3.1415/2,vec3(1.f,0.f,0.f) );
    mFloorShadowedBatch->draw();
    gl::popMatrices();
    
    
    
    gl::pushMatrices();
 

    mGlsl ->uniform( "uShadowMap", 0 );
     mGlsl ->uniform( "uGradientMap", 1 );
    mGlsl ->uniform( "uLightPos", mvLightPos );
    mGlsl ->uniform( "uShadowMatrix", shadowMatrix );
    gl::ScopedTextureBind texScope2( mShadowMapTex, (uint8_t) 0 );
    gl::ScopedTextureBind texScope3( gradientMap, (uint8_t) 1 );
    mBatch->drawInstanced( numParticle );
    
    gl::popMatrices();
    gl::disableDepthRead();
    gl::disableDepthWrite();
    
    
}






void ParticleHandler::reset()
{
    for(auto p:particles)
    {
    
        p->position =p->positionStart;
        if(glm::linearRand(0.f, 1.f)>0.9)
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
    console()<<  meshRef->getNumIndices ()<<endl;;
    
    return  meshRef;
}
void ParticleHandler::renderDepthFbo()
{
    gl::enableDepthRead();
    gl::enableDepthWrite();
    gl::enable( GL_POLYGON_OFFSET_FILL );
    glPolygonOffset( 2.0f, 2.0f );
    
    // Render scene to fbo from the view of the light
    gl::ScopedFramebuffer fbo( mFbo );
    gl::ScopedViewport viewport( vec2( 0.0f ), mFbo->getSize() );
    gl::clear( Color::black() );
    gl::color( Color::white() );
     gl::pushMatrices();
    gl::setMatrices( mLightCam );
    
   
    
    mBatchPlain->drawInstanced( numParticle );
    
    gl::popMatrices();
    
    // Disable polygon offset for final render
    gl::disable( GL_POLYGON_OFFSET_FILL );
    gl::disableDepthRead();
    gl::disableDepthWrite();

}
void ParticleHandler::setupShadow()
{
    mLightPos.x=screenWidth / 2+500;
    mLightPos.y=screenHeight / 2+500;
    mLightPos.z=-1000;
    
    gl::Texture2d::Format depthFormat;
    

    depthFormat.setInternalFormat( GL_DEPTH_COMPONENT32F );
    depthFormat.setCompareMode( GL_COMPARE_REF_TO_TEXTURE );
    depthFormat.setMagFilter( GL_LINEAR );
    depthFormat.setMinFilter( GL_LINEAR );
    depthFormat.setWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

    depthFormat.setCompareFunc( GL_LEQUAL );
    
    mShadowMapTex = gl::Texture2d::create( FBO_WIDTH, FBO_HEIGHT, depthFormat );
    
 
    gl::Fbo::Format fboFormat;
    fboFormat.attachment( GL_DEPTH_ATTACHMENT, mShadowMapTex );
    mFbo = gl::Fbo::create( FBO_WIDTH, FBO_HEIGHT, fboFormat );
    
    // Set up camera from the light's viewpoint
    mLightCam.setPerspective( 100.0f, mFbo->getAspectRatio(), 300.f,2000.0f );
    mLightCam.lookAt( mLightPos, vec3( screenWidth / 2,screenHeight / 2,0) );

    mGlslFloor = gl::GlslProg::create( loadAsset( "shadow_shader.vert" ), loadAsset( "shadow_shader.frag" ) );
    auto floor				= geom::Cube().size( 1000.0f, 0.5f, 1000.0f );
   
    mFloorShadowedBatch		= gl::Batch::create( floor, mGlslFloor );

}


