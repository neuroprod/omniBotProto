#version 150

uniform mat4	ciModelViewProjection;
uniform mat4 ciModelMatrix;

in vec4		ciPosition;

in vec2 vInstanceColor;
in mat4	vInstancePosition;


out vec2	vTexCoord0;








void main( void )
{
    
 
    gl_Position	= ciModelViewProjection *(vInstancePosition*ciPosition);
    
    vTexCoord0 = vInstanceColor;
  
    
}
