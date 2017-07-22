//
//  FloorTexture.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 19/06/2017.
//
//

#include "FloorMapping.hpp"
using namespace ci;
using namespace ci::app;
using namespace std;



void FloorMapping::load()
{

    generator.setup();
    texture =gl::Texture::create( loadImage(getAssetPath("level.png")),gl::Texture::Format().loadTopDown());
   
   
   prog = gl::GlslProg::create( gl::GlslProg::Format().vertex( loadAsset( "floor_shader.vert" ) )
                           .fragment( loadAsset( "floor_shader.frag" ) )
                          );
  
    
    prog ->uniform( "uFloorMap", 0 );
    prog->uniform( "uShadowMap", 1 );
}


void FloorMapping::setTileFloorMesh(LevelTileRef tile,int numTiles)
{
    vector<vec3> posTemp;
    float size2 =tile->size/2.f;
    posTemp.push_back(vec3(-size2,-size2,0));
    posTemp.push_back(vec3(size2,-size2,0));
    posTemp.push_back(vec3(-size2,size2,0));
    posTemp.push_back(vec3(size2,size2,0));
    
    vector<vec2> uvTemp;
    float  uvStep =1.f/numTiles;
    
    float uvOffX = tile->xR*uvStep;
     float uvOffY = tile->yR*uvStep;
    
    uvTemp.push_back(vec2(uvOffX,uvOffY));
    uvTemp.push_back(vec2(uvOffX+uvStep,uvOffY));
    uvTemp.push_back(vec2(uvOffX,uvOffY+uvStep));
    uvTemp.push_back(vec2(uvOffX+uvStep,uvOffY+uvStep));

    
    vector<unsigned short> indexTemp;
    
    indexTemp.push_back(0);
    indexTemp.push_back(1);
    indexTemp.push_back(2);
    
    indexTemp.push_back(1);
    indexTemp.push_back(3);
    indexTemp.push_back(2);
    
    
    auto vertices = vector<VertexDataFloor>();
    
    for(int i=0;i<posTemp.size();i++)
    {
        VertexDataFloor right;
        right.position =posTemp[i];
        right.uv =uvTemp[i];
        

        vertices.push_back(right);
        
        
    }

    
    
    
       auto layout = geom::BufferLayout();
    
    layout.append(geom::Attrib::POSITION, 3, sizeof(VertexDataFloor), offsetof(VertexDataFloor, position));
    layout.append(geom::Attrib::TEX_COORD_0, 2, sizeof(VertexDataFloor), offsetof(VertexDataFloor, uv));
    
    // upload your data to the GPU
    auto buffer = gl::Vbo::create(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);
    
    
  
    vector<gl::VboMesh::Layout> bufferLayout = {
        gl::VboMesh::Layout().usage(  GL_STATIC_DRAW ).attrib( geom::Attrib::POSITION, 3 ),
        gl::VboMesh::Layout().usage( GL_STATIC_DRAW ).attrib( geom::Attrib::TEX_COORD_0, 2 )
    };
    
    
    gl::VboMeshRef meshRef =gl::VboMesh::create(vertices.size(), GL_TRIANGLES, {{layout, buffer}},indexTemp.size());
    meshRef->bufferIndices(indexTemp.size()*sizeof(unsigned short), indexTemp.data());

    tile->meshFloor = meshRef;
    
}
void FloorMapping::startDraw(ci::gl::FboRef shadowFBO,ci::mat4 &shadowMatrix, PlayerLevel &playerLevel)
{
    
    
    prog->bind();
    generator.mFbo->bindTexture(0);
    
    shadowFBO->getDepthTexture()->bind(1);
   
    prog->uniform( "uShadowMatrix",  shadowMatrix );
    
   
    
    gl::pushMatrices();
    gl::translate( playerLevel.player->playerWorldOffset);
    
    for(auto tile : playerLevel.playerTiles)
    {
        tile->drawFloor();
    }
    
    
    gl::popMatrices();
    
    
    
    stopDraw();
    shadowFBO->getDepthTexture()->unbind();


}
void FloorMapping::stopDraw()
{
    texture->unbind();
     gl::getStockShader( gl::ShaderDef().color() )->bind() ;
}


