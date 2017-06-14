#version 150

uniform mat4	ciModelViewProjection;


in vec4	ciPosition;


in mat4	vInstancePosition;















void main( void )
{
  
   
    gl_Position	= ciModelViewProjection *(vInstancePosition*ciPosition);
    
    
   
}
