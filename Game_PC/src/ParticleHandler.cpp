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
    


    
    leaveMap=gl::Texture::create( loadImage( loadAsset("leave.png") ) );
    gradientMap=gl::Texture::create( loadImage( loadAsset("gradient.png") ) );
    
    mGlsl = gl::GlslProg::create( loadAsset( "shader.vert" ), loadAsset( "shader.frag" ) );
    mGlsl->uniform( "uDiffuseMap", 0 );
     mGlsl->uniform( "uGradientMap", 1 );
    
     mGlslPlain = gl::GlslProg::create( loadAsset( "shaderPlain.vert" ), loadAsset( "shaderPlain.frag" ) );
    //gl::VboMeshRef mesh = gl::VboMesh::create( ci::geom::Circle().radius (3) );
    
    gl::VboMeshRef mesh= buildVBOMesh();
    
    float sqrRadius =radius*radius;
    std::vector<vec4> positions;
     std::vector<vec4> axsises;
    std::vector<float> colors;
    ci::Perlin pnois=Perlin();
                            
                            
                            
       for(int i =0;i< numParticle;i++)
    {
        ParticleRef p= Particle::create();
        glm::vec2 randomPos = glm::vec2(glm::linearRand(-radius, radius),glm::linearRand(-radius, radius));
        
        
        bool found =false;
        while(!found){
        
            if(glm::length2( randomPos)< sqrRadius)
            {
                found =true;
            }else
            {
            
                randomPos = glm::vec2(glm::linearRand(-radius, radius),glm::linearRand(-radius, radius));

            
            }
        
            
            
        }
        vec4 axsis;
        
        axsis.x=glm::linearRand(-1, 1);
        axsis.y=glm::linearRand(-1, 1);
        axsis.z =glm::linearRand(-1, 1);
        axsis.w =glm::linearRand(0.8, 1.2);
        axsises.push_back(axsis);
        
        p->friction =glm::linearRand(0.95f, 0.98f);
        
        float noise =( pnois.fBm(randomPos.x/400, randomPos.y/400)+0.5f)*2;
       
        colors.push_back(noise +glm::linearRand(0.0f, 0.01f));
        
       p->position.x  =randomPos.x+centerPos.x;
       p->position.y  =randomPos.y+centerPos.y;
        p->position.z =glm::linearRand(0.0f, 5.f);
       p->position.w =glm::linearRand(0.0f, 7.f);
        p->positionStart=  p->position;
        positions.push_back(p->position);
        
        particles.push_back(p);
    
    
    
    }
    
    mInstanceDataVbo = gl::Vbo::create( GL_ARRAY_BUFFER, positions.size() * sizeof(vec4), positions.data(), GL_DYNAMIC_DRAW );
    geom::BufferLayout instanceDataLayout;
    instanceDataLayout.append( geom::Attrib::CUSTOM_0, 4, 0, 0, 1 );
    mesh->appendVbo( instanceDataLayout, mInstanceDataVbo );
    
    
    
    
    mInstanceDataVboColor = gl::Vbo::create( GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
    geom::BufferLayout instanceDataLayoutColor;
    instanceDataLayoutColor.append( geom::Attrib::CUSTOM_1, 1, 0, 0, 1 );
    mesh->appendVbo( instanceDataLayoutColor, mInstanceDataVboColor );
    
    mInstanceDataVboAx = gl::Vbo::create( GL_ARRAY_BUFFER, axsises.size() * sizeof(vec4), axsises.data(), GL_STATIC_DRAW);
    geom::BufferLayout instanceDataLayoutAx;
    instanceDataLayoutAx.append( geom::Attrib::CUSTOM_2, 4, 0, 0, 1 );
    mesh->appendVbo( instanceDataLayoutAx, mInstanceDataVboColor );
    // now add it to the VboMesh we already made of the Teapot
    
    
    // and finally, build our batch, mapping our CUSTOM_0 attribute to the "vInstancePosition" GLSL vertex attribute
    mBatch = gl::Batch::create( mesh, mGlsl, { { geom::Attrib::CUSTOM_0, "vInstancePosition" },{ geom::Attrib::CUSTOM_1, "vInstanceColor" },{ geom::Attrib::CUSTOM_2, "vInstanceAxisScale" } } );
    
    
     mBatchPlain = gl::Batch::create( mesh, mGlslPlain, { { geom::Attrib::CUSTOM_0, "vInstancePosition" },{ geom::Attrib::CUSTOM_1, "vInstanceColor" },{ geom::Attrib::CUSTOM_2, "vInstanceAxisScale" } } );
}

void ParticleHandler::update(PlayerRef player)
{
    glm::vec2 pos=  player->drawPosition2D;
    glm::vec2 speed= player->moveSpeed2D;
    float speedSize = glm::length(speed);
    float robotSize  =player->robotSize;
    float robotsize2 = pow(robotSize,2.0f);
  
    vec4 *positions = (vec4*)mInstanceDataVbo->mapReplace();

    
    for(auto p:particles)
    {
    
        glm::vec2 pPos =glm::vec2(p->position.x,p->position.y);
        float distance2 = glm::distance2(pos, pPos);
        if(distance2<robotsize2 )
        {
        
            p->hit  =true;
            glm::vec2 hitDir =pPos -pos;
            float hitSize =robotSize -glm::length( hitDir);
            hitDir = glm::normalize(hitDir);
            glm::vec2 moveDir =hitDir*(hitSize+glm::linearRand(0.f, 8.f));
            p->position.x += moveDir.x;
            p->position.y += moveDir.y;
            
            
            
            float rand  =glm::linearRand(0.f, speedSize/30.f);
            
            p->speed.x += moveDir.x*rand;
            p->speed.y += moveDir.y*rand;
        
        }else
        {
        
            p->hit =false;
           
        }
    
        
        
        
        
        
        
        p->speed*=p->friction;
        p->position.x+=p->speed.x;
        p->position.y+=p->speed.y;
         p->position.z+=p->speed.z;
         p->position.w+=(p->speed.x+p->speed.y)/100.f;
        *positions++  =p->position ;
    
    }
    
    mInstanceDataVbo->unmap();
    
    renderDepthFbo();
}

void ParticleHandler::draw()
{
    gl::enableDepthRead();
    gl::enableDepthWrite();
     gl::setMatrices( camera);
    
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
    leaveMap->bind(0);
    gradientMap->bind(1);
    mGlsl ->uniform( "uShadowMap", 3 );
    mGlsl ->uniform( "uLightPos", mvLightPos );
    mGlsl ->uniform( "uShadowMatrix", shadowMatrix );
 gl::ScopedTextureBind texScope2( mShadowMapTex, (uint8_t) 3 );
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
    }
    
}

gl::VboMeshRef ParticleHandler::buildVBOMesh()
{
    auto vertices = vector<VertexData>();
    float size =20;
    
    VertexData top;
    top.position.y=-size;
    
    vertices.push_back(top);
    
    VertexData left;
    left.position.x=size;
    left.position.y=0;
    left.position.z=5;
    
    left.texture_uv.x=1;
    vertices.push_back(left);
    
    
    VertexData right;
    right.position.x=-size;
    right.position.y=0;
    right.position.z=5;
    right.texture_uv.x=0;
    right.texture_uv.y=1;
    
    
    vertices.push_back(right);
    vertices.push_back(left);
    vertices.push_back(right);
 
    VertexData bottom;
    bottom.position.y=size;
     bottom.texture_uv.y=1;
      bottom.texture_uv.x=1;
     vertices.push_back(bottom);
    
    auto layout = geom::BufferLayout();
    layout.append(geom::Attrib::POSITION, 3, sizeof(VertexData), offsetof(VertexData, position));
    layout.append(geom::Attrib::TEX_COORD_0, 2, sizeof(VertexData), offsetof(VertexData, texture_uv));
    
    // upload your data to the GPU
    auto buffer = gl::Vbo::create(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);
    
    // construct a VAO that describes the data in the buffer according to your layout.
   return  gl::VboMesh::create(vertices.size(), GL_TRIANGLES, {{layout, buffer}});
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
    mLightPos.x=screenWidth / 2+200;
    mLightPos.y=screenHeight / 2+200;
    mLightPos.z=-500;
    
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
    mLightCam.setPerspective( 100.0f, mFbo->getAspectRatio(), 300.f,800.0f );
    mLightCam.lookAt( mLightPos, vec3( screenWidth / 2,screenHeight / 2,0) );

    mGlslFloor = gl::GlslProg::create( loadAsset( "shadow_shader.vert" ), loadAsset( "shadow_shader.frag" ) );
    auto floor				= geom::Cube().size( 1000.0f, 0.5f, 1000.0f );
   
    mFloorShadowedBatch		= gl::Batch::create( floor, mGlslFloor );

}


